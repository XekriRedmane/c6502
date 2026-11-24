SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_test_non_constant_and_type_conversion__0	EQU	$82
__local_test_partial__0	EQU	$82
__local_test_simple__0	EQU	$82
__local_test_non_constant_and_type_conversion__1	EQU	$83
__local_test_partial__1	EQU	$83
__local_test_simple__1	EQU	$83
__local_test_non_constant_and_type_conversion__2	EQU	$84
__local_test_partial__2	EQU	$84
__local_test_preserve_stack__arr_3	EQU	$84
__local_test_simple__2	EQU	$84
__local_test_non_constant_and_type_conversion__3	EQU	$85
__local_test_partial__3	EQU	$85
__local_test_preserve_stack__arr_2	EQU	$85
__local_test_simple__3	EQU	$85
__local_test_non_constant_and_type_conversion__4	EQU	$86
__local_test_partial__4	EQU	$86
__local_test_preserve_stack__arr_0	EQU	$86
__local_test_simple__4	EQU	$86
__local_test_non_constant_and_type_conversion__5	EQU	$87
__local_test_partial__5	EQU	$87
__local_test_preserve_stack__arr_1	EQU	$87
__local_test_simple__j_1	EQU	$87
__local_test_non_constant_and_type_conversion__6	EQU	$88
__local_test_partial__k_1	EQU	$88
__local_test_simple__j_0	EQU	$88
__local_test_non_constant_and_type_conversion__7	EQU	$89
__local_test_partial__k_0	EQU	$89
__local_test_simple__i_1	EQU	$89
__local_test_non_constant_and_type_conversion__8	EQU	$8A
__local_test_partial__j	EQU	$8A
__local_test_simple__i_0	EQU	$8A
__local_test_non_constant_and_type_conversion__9	EQU	$8B
__local_test_partial__i_1	EQU	$8B
__local_test_simple__arr	EQU	$8B
__local_test_non_constant_and_type_conversion__negative_four	EQU	$8C
__local_test_partial__i_0	EQU	$8C
__local_test_partial__expected_1	EQU	$8D
__local_test_non_constant_and_type_conversion__arr	EQU	$8E
__local_test_partial__expected_0	EQU	$8E
__local_test_partial__first_half_only	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_non_constant_and_type_conversion params= locals=__local_test_non_constant_and_type_conversion__0,__local_test_non_constant_and_type_conversion__1,__local_test_non_constant_and_type_conversion__2,__local_test_non_constant_and_type_conversion__3,__local_test_non_constant_and_type_conversion__4,__local_test_non_constant_and_type_conversion__5,__local_test_non_constant_and_type_conversion__6,__local_test_non_constant_and_type_conversion__7,__local_test_non_constant_and_type_conversion__8,__local_test_non_constant_and_type_conversion__9,__local_test_non_constant_and_type_conversion__negative_four,__local_test_non_constant_and_type_conversion__11,__local_test_non_constant_and_type_conversion__arr,__local_test_non_constant_and_type_conversion__13,__local_test_non_constant_and_type_conversion__14,__local_test_non_constant_and_type_conversion__15,__local_test_non_constant_and_type_conversion__16,__local_test_non_constant_and_type_conversion__17,__local_test_non_constant_and_type_conversion__18,__local_test_non_constant_and_type_conversion__19,__local_test_non_constant_and_type_conversion__20,__local_test_non_constant_and_type_conversion__21,__local_test_non_constant_and_type_conversion__22,__local_test_non_constant_and_type_conversion__23,__local_test_non_constant_and_type_conversion__24,__local_test_non_constant_and_type_conversion__25,__local_test_non_constant_and_type_conversion__26,__local_test_non_constant_and_type_conversion__27,__local_test_non_constant_and_type_conversion__28,__local_test_non_constant_and_type_conversion__29,__local_test_non_constant_and_type_conversion__30,__local_test_non_constant_and_type_conversion__31,__local_test_non_constant_and_type_conversion__32,__local_test_non_constant_and_type_conversion__33,__local_test_non_constant_and_type_conversion__34,__local_test_non_constant_and_type_conversion__35,__local_test_non_constant_and_type_conversion__36,__local_test_non_constant_and_type_conversion__37,__local_test_non_constant_and_type_conversion__38,__local_test_non_constant_and_type_conversion__39,__local_test_non_constant_and_type_conversion__40,__local_test_non_constant_and_type_conversion__41,__local_test_non_constant_and_type_conversion__42,__local_test_non_constant_and_type_conversion__43,__local_test_non_constant_and_type_conversion__44,__local_test_non_constant_and_type_conversion__45,__local_test_non_constant_and_type_conversion__46,__local_test_non_constant_and_type_conversion__47,__local_test_non_constant_and_type_conversion__48,__local_test_non_constant_and_type_conversion__49,__local_test_non_constant_and_type_conversion__50,__local_test_non_constant_and_type_conversion__51,__local_test_non_constant_and_type_conversion__52,__local_test_non_constant_and_type_conversion__53,__local_test_non_constant_and_type_conversion__54,__local_test_non_constant_and_type_conversion__55,__local_test_non_constant_and_type_conversion__56,__local_test_non_constant_and_type_conversion__57,__local_test_non_constant_and_type_conversion__58,__local_test_non_constant_and_type_conversion__59 indirect=false in_cycle=false
; def test_partial params= locals=__local_test_partial__0,__local_test_partial__1,__local_test_partial__2,__local_test_partial__3,__local_test_partial__4,__local_test_partial__5,__local_test_partial__k_1,__local_test_partial__k_0,__local_test_partial__j,__local_test_partial__i_1,__local_test_partial__i_0,__local_test_partial__expected_1,__local_test_partial__expected_0,__local_test_partial__first_half_only,__local_test_partial__7,__local_test_partial__8,__local_test_partial__9,__local_test_partial__10,__local_test_partial__11,__local_test_partial__12,__local_test_partial__13,__local_test_partial__14,__local_test_partial__15,__local_test_partial__16,__local_test_partial__17,__local_test_partial__18,__local_test_partial__19,__local_test_partial__20,__local_test_partial__21,__local_test_partial__22,__local_test_partial__23,__local_test_partial__24,__local_test_partial__25,__local_test_partial__26,__local_test_partial__27,__local_test_partial__28,__local_test_partial__29,__local_test_partial__30,__local_test_partial__31,__local_test_partial__32,__local_test_partial__33,__local_test_partial__34,__local_test_partial__35,__local_test_partial__36,__local_test_partial__37,__local_test_partial__38,__local_test_partial__39,__local_test_partial__40,__local_test_partial__41,__local_test_partial__42,__local_test_partial__43,__local_test_partial__44,__local_test_partial__45,__local_test_partial__46,__local_test_partial__47,__local_test_partial__48,__local_test_partial__49,__local_test_partial__50,__local_test_partial__51,__local_test_partial__52,__local_test_partial__53,__local_test_partial__54,__local_test_partial__55,__local_test_partial__56,__local_test_partial__57,__local_test_partial__58,__local_test_partial__59,__local_test_partial__60,__local_test_partial__61,__local_test_partial__62,__local_test_partial__63,__local_test_partial__64,__local_test_partial__65,__local_test_partial__66,__local_test_partial__67,__local_test_partial__68,__local_test_partial__69,__local_test_partial__70,__local_test_partial__71,__local_test_partial__72,__local_test_partial__73,__local_test_partial__74,__local_test_partial__75,__local_test_partial__76,__local_test_partial__77,__local_test_partial__78,__local_test_partial__79,__local_test_partial__80,__local_test_partial__81,__local_test_partial__82,__local_test_partial__83,__local_test_partial__84,__local_test_partial__85,__local_test_partial__86,__local_test_partial__87,__local_test_partial__88,__local_test_partial__89,__local_test_partial__90,__local_test_partial__91,__local_test_partial__92,__local_test_partial__93,__local_test_partial__94,__local_test_partial__95,__local_test_partial__96,__local_test_partial__97,__local_test_partial__98,__local_test_partial__99,__local_test_partial__100,__local_test_partial__101 indirect=false in_cycle=false
; def test_preserve_stack params= locals=__local_test_preserve_stack__arr_5,__local_test_preserve_stack__arr_4,__local_test_preserve_stack__arr_3,__local_test_preserve_stack__arr_2,__local_test_preserve_stack__arr_0,__local_test_preserve_stack__arr_1 indirect=false in_cycle=false
; def test_simple params= locals=__local_test_simple__0,__local_test_simple__1,__local_test_simple__2,__local_test_simple__3,__local_test_simple__4,__local_test_simple__j_1,__local_test_simple__j_0,__local_test_simple__i_1,__local_test_simple__i_0,__local_test_simple__arr,__local_test_simple__6,__local_test_simple__7,__local_test_simple__8,__local_test_simple__9,__local_test_simple__10,__local_test_simple__11,__local_test_simple__12,__local_test_simple__13,__local_test_simple__14,__local_test_simple__15,__local_test_simple__16,__local_test_simple__17,__local_test_simple__18,__local_test_simple__19,__local_test_simple__20,__local_test_simple__21,__local_test_simple__22 indirect=false in_cycle=false
; def three params= locals= indirect=false in_cycle=false
; call main -> test_non_constant_and_type_conversion
; call main -> test_partial
; call main -> test_preserve_stack
; call main -> test_simple
; call test_non_constant_and_type_conversion -> three
; call test_preserve_stack -> three
; item test_simple section=.text local=false zero=false
; item test_simple@tail1 section=.text local=true zero=false
; item test_simple@tail0 section=.text local=true zero=false
; item test_partial section=.text local=false zero=false
; item test_non_constant_and_type_conversion section=.text local=false zero=false
; item test_non_constant_and_type_conversion@run0 section=.text local=true zero=false
; item three section=.text local=false zero=false
; item test_preserve_stack section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @9.x section=.data local=true zero=false
; item one section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

test_simple:
   SUBROUTINE

   LDA   #$01
   STA   __local_test_simple__arr
   LDA   #$00
   STA   __local_test_simple__arr+1
   LDY   #$02
   STY   __local_test_simple__arr+2
   STA   __local_test_simple__arr+3
   INY
   STY   __local_test_simple__arr+4
   STA   __local_test_simple__arr+5
   INY
   STY   __local_test_simple__arr+6
   STA   __local_test_simple__arr+7
   INY
   STY   __local_test_simple__arr+8
   STA   __local_test_simple__arr+9
   INY
   STY   __local_test_simple__arr+10
   STA   __local_test_simple__arr+11
   INY
   STY   __local_test_simple__arr+12
   STA   __local_test_simple__arr+13
   INY
   STY   __local_test_simple__arr+14
   STA   __local_test_simple__arr+15
   INY
   STY   __local_test_simple__arr+16
   STA   __local_test_simple__arr+17
   STA   __local_test_simple__i_0
   LDY   #$00
   STY   __local_test_simple__i_1
.loop@0_start:
   LDA   __local_test_simple__i_0
   SEC
   SBC   #$03
   LDA   __local_test_simple__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@3
   JMP   .loop@0_break
.lb_skip@3:
   LDA   #$00
   STA   __local_test_simple__j_0
   LDY   #$00
   STY   __local_test_simple__j_1
.loop@1_start:
   LDA   __local_test_simple__j_0
   SEC
   SBC   #$03
   LDA   __local_test_simple__j_1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BMI   .lb_skip@2
   JMP   .loop@1_break
.lb_skip@2:
   LDA   #>__local_test_simple__arr
   STA   __local_test_simple__3
   LDA   __local_test_simple__i_0
   ASL
   STA   __local_test_simple__1
   ASL
   CLC
   ADC   __local_test_simple__1
   STA   __local_test_simple__0
   LDA   #<__local_test_simple__arr
   CLC
   ADC   __local_test_simple__0
   STA   __local_test_simple__1
   BCC   .carry_inc@0
   INC   __local_test_simple__3
.carry_inc@0:
   LDA   __local_test_simple__j_0
   ASL
   CLC
   ADC   __local_test_simple__1
   STA   __local_test_simple__0
   LDA   __local_test_simple__3
   ADC   #$00
   STA   __local_test_simple__1
   LDY   #$00
   LDA   (__local_test_simple__0),Y
   STA   __local_test_simple__4
   INY
   LDA   (__local_test_simple__0),Y
   STA   __local_test_simple__3
   LDA   __local_test_simple__i_0
   ASL
   STA   __local_test_simple__1
   LDA   __local_test_simple__i_1
   ROL
   STA   __local_test_simple__2
   LDA   __local_test_simple__1
   CLC
   ADC   __local_test_simple__i_0
   STA   __local_test_simple__0
   LDA   __local_test_simple__2
   ADC   __local_test_simple__i_1
   STA   __local_test_simple__1
   LDA   __local_test_simple__0
   CLC
   ADC   __local_test_simple__j_0
   STA   __local_test_simple__0
   LDA   __local_test_simple__1
   ADC   __local_test_simple__j_1
   STA   __local_test_simple__1
   INC   __local_test_simple__0
   BNE   .inc_done@0
   INC   __local_test_simple__1
.inc_done@0:
   LDA   __local_test_simple__3
   CMP   __local_test_simple__1
   BNE   .jcmp_skip@2
   LDA   __local_test_simple__4
   CMP   __local_test_simple__0
   BEQ   .if_end@0
.jcmp_skip@2:
   JMP   test_simple@tail1
.if_end@0:
   INC   __local_test_simple__j_0
   BEQ   .lb_skip@1
   JMP   .loop@1_start
.lb_skip@1:
   INC   __local_test_simple__j_1
   JMP   .loop@1_start
.loop@1_break:
   INC   __local_test_simple__i_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_test_simple__i_1
   JMP   .loop@0_start
.loop@0_break:
   JMP   test_simple@tail0

test_simple@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_simple@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_partial:
   SUBROUTINE

   LDA   #$01
   STA   __local_test_partial__first_half_only
   LDA   #$00
   STA   __local_test_partial__first_half_only+1
   LDY   #$02
   STY   __local_test_partial__first_half_only+2
   STA   __local_test_partial__first_half_only+3
   INY
   STY   __local_test_partial__first_half_only+4
   STA   __local_test_partial__first_half_only+5
   LDA   #$12
   STA   __local_test_partial__4
.clear@1:
   DEC   __local_test_partial__4
   LDA   __local_test_partial__4
   CLC
   ADC   #<__local_test_partial__first_half_only
   STA   __local_test_partial__0
   LDA   #$00
   ADC   #>__local_test_partial__first_half_only
   STA   __local_test_partial__1
   LDA   #$00
   LDY   #$06
   STA   (__local_test_partial__0),Y
   LDA   __local_test_partial__4
   BNE   .clear@1
   LDA   #$04
   STA   __local_test_partial__first_half_only+24
   LDA   #$00
   STA   __local_test_partial__first_half_only+25
   DEY
   STY   __local_test_partial__first_half_only+26
   STA   __local_test_partial__first_half_only+27
   INY
   STY   __local_test_partial__first_half_only+28
   STA   __local_test_partial__first_half_only+29
   LDA   #$42
   STA   __local_test_partial__4
.clear@2:
   DEC   __local_test_partial__4
   LDA   __local_test_partial__4
   CLC
   ADC   #<__local_test_partial__first_half_only
   STA   __local_test_partial__0
   LDA   #$00
   ADC   #>__local_test_partial__first_half_only
   STA   __local_test_partial__1
   LDA   #$00
   LDY   #$1E
   STA   (__local_test_partial__0),Y
   LDA   __local_test_partial__4
   BNE   .clear@2
   LDA   #$01
   STA   __local_test_partial__expected_0
   LDA   #$00
   STA   __local_test_partial__expected_1
   STA   __local_test_partial__i_0
   LDY   #$00
   STY   __local_test_partial__i_1
.loop@2_start:
   LDA   __local_test_partial__i_0
   SEC
   SBC   #$04
   LDA   __local_test_partial__i_1
   SBC   #$00
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BMI   .lb_skip@9
   JMP   .loop@2_break
.lb_skip@9:
   LDA   #$00
   STA   __local_test_partial__j
   LDX   #$00
.loop@3_start:
   LDA   __local_test_partial__j
   SEC
   SBC   #$02
   TXA
   SBC   #$00
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BMI   .lb_skip@8
   JMP   .loop@3_break
.lb_skip@8:
   LDA   #$00
   STA   __local_test_partial__k_0
   LDY   #$00
   STY   __local_test_partial__k_1
.loop@4_start:
   LDA   __local_test_partial__k_0
   SEC
   SBC   #$06
   LDA   __local_test_partial__k_1
   SBC   #$00
   BVC   .jcmp_novf@5
   EOR   #$80
.jcmp_novf@5:
   BMI   .lb_skip@7
   JMP   .loop@4_break
.lb_skip@7:
   LDA   #>__local_test_partial__first_half_only
   STA   __local_test_partial__5
   LDA   __local_test_partial__i_0
   ASL
   ASL
   ASL
   STA   __local_test_partial__1
   ASL
   CLC
   ADC   __local_test_partial__1
   STA   __local_test_partial__0
   LDA   #<__local_test_partial__first_half_only
   CLC
   ADC   __local_test_partial__0
   STA   __local_test_partial__2
   BCC   .carry_inc@1
   INC   __local_test_partial__5
.carry_inc@1:
   LDA   __local_test_partial__j
   ASL
   ASL
   STA   __local_test_partial__1
   ASL
   CLC
   ADC   __local_test_partial__1
   CLC
   ADC   __local_test_partial__2
   STA   __local_test_partial__1
   BCC   .carry_inc@2
   INC   __local_test_partial__5
.carry_inc@2:
   LDA   __local_test_partial__k_0
   ASL
   CLC
   ADC   __local_test_partial__1
   STA   __local_test_partial__2
   LDA   __local_test_partial__5
   ADC   #$00
   STA   __local_test_partial__3
   LDY   #$00
   LDA   (__local_test_partial__2),Y
   STA   __local_test_partial__1
   INY
   LDA   (__local_test_partial__2),Y
   STA   __local_test_partial__0
   LDA   #$01
   SEC
   SBC   __local_test_partial__i_0
   LDA   #$00
   SBC   __local_test_partial__i_1
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BMI   .test_partial@jthread@1
   TXA
   BMI   .jcmp_neg@7
   ORA   __local_test_partial__j
   BNE   .test_partial@jthread@1
.jcmp_neg@7:
   LDA   #$02
   SEC
   SBC   __local_test_partial__k_0
   LDA   #$00
   SBC   __local_test_partial__k_1
   BVC   .jcmp_novf@8
   EOR   #$80
.jcmp_novf@8:
   BPL   .if_else@8
.test_partial@jthread@1:
   LDA   __local_test_partial__1
   ORA   __local_test_partial__0
   BEQ   .if_end@7
   JMP   test_simple@tail1
.if_else@8:
   LDA   __local_test_partial__0
   CMP   __local_test_partial__expected_1
   BNE   .jcmp_skip@9
   LDA   __local_test_partial__1
   CMP   __local_test_partial__expected_0
   BEQ   .if_end@10
.jcmp_skip@9:
   LDX   #$00
   TXA
   RTS
.if_end@10:
   INC   __local_test_partial__expected_0
   BNE   .inc_done@3
   INC   __local_test_partial__expected_1
.inc_done@3:
.if_end@7:
   INC   __local_test_partial__k_0
   BEQ   .lb_skip@6
   JMP   .loop@4_start
.lb_skip@6:
   INC   __local_test_partial__k_1
   JMP   .loop@4_start
.loop@4_break:
   INC   __local_test_partial__j
   BEQ   .lb_skip@5
   JMP   .loop@3_start
.lb_skip@5:
   INX
   JMP   .loop@3_start
.loop@3_break:
   INC   __local_test_partial__i_0
   BEQ   .lb_skip@4
   JMP   .loop@2_start
.lb_skip@4:
   INC   __local_test_partial__i_1
   JMP   .loop@2_start
.loop@2_break:
   JMP   test_simple@tail0

test_non_constant_and_type_conversion:
   SUBROUTINE

   LDA   #$FC
   STA   __local_test_non_constant_and_type_conversion__negative_four
   LDA   #$FF
   STA   __local_test_non_constant_and_type_conversion__negative_four+1
   LDA   #<__local_test_non_constant_and_type_conversion__negative_four
   STA   __local_test_non_constant_and_type_conversion__8
   LDA   #>__local_test_non_constant_and_type_conversion__negative_four
   STA   __local_test_non_constant_and_type_conversion__9
   JSR   test_non_constant_and_type_conversion@run0
   JSR   i2d
   LDA   HARGS+2
   STA   __local_test_non_constant_and_type_conversion__arr
   LDA   HARGS+3
   STA   __local_test_non_constant_and_type_conversion__arr+1
   LDA   HARGS+4
   STA   __local_test_non_constant_and_type_conversion__arr+2
   LDA   HARGS+5
   STA   __local_test_non_constant_and_type_conversion__arr+3
   LDA   HARGS+6
   STA   __local_test_non_constant_and_type_conversion__arr+4
   LDA   HARGS+7
   STA   __local_test_non_constant_and_type_conversion__arr+5
   LDA   HARGS+8
   STA   __local_test_non_constant_and_type_conversion__arr+6
   LDA   HARGS+9
   STA   __local_test_non_constant_and_type_conversion__arr+7
   LDY   #$00
   LDA   (__local_test_non_constant_and_type_conversion__8),Y
   STA   HARGS+2
   INY
   LDA   (__local_test_non_constant_and_type_conversion__8),Y
   STA   HARGS+3
   JSR   test_non_constant_and_type_conversion@run0
   JSR   sdivmod16
   LDA   HARGS+4
   STA   HARGS
   LDA   HARGS+5
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   STA   __local_test_non_constant_and_type_conversion__arr+8
   LDA   HARGS+3
   STA   __local_test_non_constant_and_type_conversion__arr+9
   LDA   HARGS+4
   STA   __local_test_non_constant_and_type_conversion__arr+10
   LDA   HARGS+5
   STA   __local_test_non_constant_and_type_conversion__arr+11
   LDA   HARGS+6
   STA   __local_test_non_constant_and_type_conversion__arr+12
   LDA   HARGS+7
   STA   __local_test_non_constant_and_type_conversion__arr+13
   LDA   HARGS+8
   STA   __local_test_non_constant_and_type_conversion__arr+14
   LDA   HARGS+9
   STA   __local_test_non_constant_and_type_conversion__arr+15
   JSR   three
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   JSR   ul2d
   LDA   HARGS+8
   STA   __local_test_non_constant_and_type_conversion__arr+20
   LDA   HARGS+9
   STA   __local_test_non_constant_and_type_conversion__arr+21
   LDA   HARGS+10
   STA   __local_test_non_constant_and_type_conversion__arr+22
   LDA   HARGS+11
   STA   __local_test_non_constant_and_type_conversion__arr+23
   LDA   HARGS+4
   STA   __local_test_non_constant_and_type_conversion__arr+16
   LDA   HARGS+5
   STA   __local_test_non_constant_and_type_conversion__arr+17
   LDA   HARGS+6
   STA   __local_test_non_constant_and_type_conversion__arr+18
   LDA   HARGS+7
   STA   __local_test_non_constant_and_type_conversion__arr+19
   LDA   #$18
   STA   __local_test_non_constant_and_type_conversion__4
.clear@11:
   DEC   __local_test_non_constant_and_type_conversion__4
   LDA   __local_test_non_constant_and_type_conversion__4
   CLC
   ADC   #<__local_test_non_constant_and_type_conversion__arr
   STA   __local_test_non_constant_and_type_conversion__0
   LDA   #$00
   ADC   #>__local_test_non_constant_and_type_conversion__arr
   STA   __local_test_non_constant_and_type_conversion__1
   LDA   #$00
   LDY   #$18
   STA   (__local_test_non_constant_and_type_conversion__0),Y
   LDA   __local_test_non_constant_and_type_conversion__4
   BNE   .clear@11
   LDA   __local_test_non_constant_and_type_conversion__arr
   STA   __local_test_non_constant_and_type_conversion__7
   LDA   __local_test_non_constant_and_type_conversion__arr+1
   STA   __local_test_non_constant_and_type_conversion__6
   LDA   __local_test_non_constant_and_type_conversion__arr+2
   STA   __local_test_non_constant_and_type_conversion__5
   LDA   __local_test_non_constant_and_type_conversion__arr+3
   STA   __local_test_non_constant_and_type_conversion__4
   LDA   __local_test_non_constant_and_type_conversion__arr+4
   STA   __local_test_non_constant_and_type_conversion__3
   LDA   __local_test_non_constant_and_type_conversion__arr+5
   STA   __local_test_non_constant_and_type_conversion__2
   LDA   __local_test_non_constant_and_type_conversion__arr+6
   STA   __local_test_non_constant_and_type_conversion__1
   LDA   __local_test_non_constant_and_type_conversion__arr+7
   CMP   #$40
   BEQ   .lb_skip@18
   JMP   .test_non_constant_and_type_conversion@jthread@1
.lb_skip@18:
   LDA   __local_test_non_constant_and_type_conversion__1
   CMP   #$59
   BEQ   .lb_skip@17
   JMP   .test_non_constant_and_type_conversion@jthread@1
.lb_skip@17:
   LDA   __local_test_non_constant_and_type_conversion__2
   BEQ   .lb_skip@16
   JMP   .test_non_constant_and_type_conversion@jthread@1
.lb_skip@16:
   LDA   __local_test_non_constant_and_type_conversion__3
   BEQ   .lb_skip@15
   JMP   .test_non_constant_and_type_conversion@jthread@1
.lb_skip@15:
   LDA   __local_test_non_constant_and_type_conversion__4
   BEQ   .lb_skip@14
   JMP   .test_non_constant_and_type_conversion@jthread@1
.lb_skip@14:
   LDA   __local_test_non_constant_and_type_conversion__5
   BEQ   .lb_skip@13
   JMP   .test_non_constant_and_type_conversion@jthread@1
.lb_skip@13:
   LDA   __local_test_non_constant_and_type_conversion__6
   BEQ   .lb_skip@12
   JMP   .test_non_constant_and_type_conversion@jthread@1
.lb_skip@12:
   LDA   __local_test_non_constant_and_type_conversion__7
   BEQ   .lb_skip@11
   JMP   .test_non_constant_and_type_conversion@jthread@1
.lb_skip@11:
   LDA   __local_test_non_constant_and_type_conversion__arr+8
   STA   __local_test_non_constant_and_type_conversion__7
   LDA   __local_test_non_constant_and_type_conversion__arr+9
   STA   __local_test_non_constant_and_type_conversion__6
   LDA   __local_test_non_constant_and_type_conversion__arr+10
   STA   __local_test_non_constant_and_type_conversion__5
   LDA   __local_test_non_constant_and_type_conversion__arr+11
   STA   __local_test_non_constant_and_type_conversion__4
   LDA   __local_test_non_constant_and_type_conversion__arr+12
   STA   __local_test_non_constant_and_type_conversion__3
   LDA   __local_test_non_constant_and_type_conversion__arr+13
   STA   __local_test_non_constant_and_type_conversion__2
   LDA   __local_test_non_constant_and_type_conversion__arr+14
   STA   __local_test_non_constant_and_type_conversion__1
   LDA   __local_test_non_constant_and_type_conversion__arr+15
   CMP   #$C0
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__1
   CMP   #$39
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__2
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__3
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__4
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__5
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__6
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__7
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__arr+16
   STA   __local_test_non_constant_and_type_conversion__7
   LDA   __local_test_non_constant_and_type_conversion__arr+17
   STA   __local_test_non_constant_and_type_conversion__6
   LDA   __local_test_non_constant_and_type_conversion__arr+18
   STA   __local_test_non_constant_and_type_conversion__5
   LDA   __local_test_non_constant_and_type_conversion__arr+19
   STA   __local_test_non_constant_and_type_conversion__4
   LDA   __local_test_non_constant_and_type_conversion__arr+20
   STA   __local_test_non_constant_and_type_conversion__3
   LDA   __local_test_non_constant_and_type_conversion__arr+21
   STA   __local_test_non_constant_and_type_conversion__2
   LDA   __local_test_non_constant_and_type_conversion__arr+22
   STA   __local_test_non_constant_and_type_conversion__1
   LDA   __local_test_non_constant_and_type_conversion__arr+23
   CMP   #$40
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__1
   CMP   #$08
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__2
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__3
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__4
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__5
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__6
   BNE   .test_non_constant_and_type_conversion@jthread@1
   LDA   __local_test_non_constant_and_type_conversion__7
   BEQ   .if_end@16
.test_non_constant_and_type_conversion@jthread@1:
   JMP   test_simple@tail1
.if_end@16:
   LDA   __local_test_non_constant_and_type_conversion__arr+24
   LDX   __local_test_non_constant_and_type_conversion__arr+25
   STX   __local_test_non_constant_and_type_conversion__6
   LDX   __local_test_non_constant_and_type_conversion__arr+26
   STX   __local_test_non_constant_and_type_conversion__5
   LDX   __local_test_non_constant_and_type_conversion__arr+27
   STX   __local_test_non_constant_and_type_conversion__4
   LDX   __local_test_non_constant_and_type_conversion__arr+28
   STX   __local_test_non_constant_and_type_conversion__3
   LDX   __local_test_non_constant_and_type_conversion__arr+29
   STX   __local_test_non_constant_and_type_conversion__2
   LDX   __local_test_non_constant_and_type_conversion__arr+30
   STX   __local_test_non_constant_and_type_conversion__1
   LDX   __local_test_non_constant_and_type_conversion__arr+31
   STX   __local_test_non_constant_and_type_conversion__0
   ORA   __local_test_non_constant_and_type_conversion__6
   ORA   __local_test_non_constant_and_type_conversion__5
   ORA   __local_test_non_constant_and_type_conversion__4
   ORA   __local_test_non_constant_and_type_conversion__3
   ORA   __local_test_non_constant_and_type_conversion__2
   ORA   __local_test_non_constant_and_type_conversion__1
   ORA   __local_test_non_constant_and_type_conversion__0
   BEQ   .lb_skip@10
   JMP   .test_non_constant_and_type_conversion@jthread@3
.lb_skip@10:
   LDA   __local_test_non_constant_and_type_conversion__arr+32
   LDX   __local_test_non_constant_and_type_conversion__arr+33
   STX   __local_test_non_constant_and_type_conversion__6
   LDX   __local_test_non_constant_and_type_conversion__arr+34
   STX   __local_test_non_constant_and_type_conversion__5
   LDX   __local_test_non_constant_and_type_conversion__arr+35
   STX   __local_test_non_constant_and_type_conversion__4
   LDX   __local_test_non_constant_and_type_conversion__arr+36
   STX   __local_test_non_constant_and_type_conversion__3
   LDX   __local_test_non_constant_and_type_conversion__arr+37
   STX   __local_test_non_constant_and_type_conversion__2
   LDX   __local_test_non_constant_and_type_conversion__arr+38
   STX   __local_test_non_constant_and_type_conversion__1
   LDX   __local_test_non_constant_and_type_conversion__arr+39
   STX   __local_test_non_constant_and_type_conversion__0
   ORA   __local_test_non_constant_and_type_conversion__6
   ORA   __local_test_non_constant_and_type_conversion__5
   ORA   __local_test_non_constant_and_type_conversion__4
   ORA   __local_test_non_constant_and_type_conversion__3
   ORA   __local_test_non_constant_and_type_conversion__2
   ORA   __local_test_non_constant_and_type_conversion__1
   ORA   __local_test_non_constant_and_type_conversion__0
   BNE   .test_non_constant_and_type_conversion@jthread@3
   LDA   __local_test_non_constant_and_type_conversion__arr+40
   LDX   __local_test_non_constant_and_type_conversion__arr+41
   STX   __local_test_non_constant_and_type_conversion__6
   LDX   __local_test_non_constant_and_type_conversion__arr+42
   STX   __local_test_non_constant_and_type_conversion__5
   LDX   __local_test_non_constant_and_type_conversion__arr+43
   STX   __local_test_non_constant_and_type_conversion__4
   LDX   __local_test_non_constant_and_type_conversion__arr+44
   STX   __local_test_non_constant_and_type_conversion__3
   LDX   __local_test_non_constant_and_type_conversion__arr+45
   STX   __local_test_non_constant_and_type_conversion__2
   LDX   __local_test_non_constant_and_type_conversion__arr+46
   STX   __local_test_non_constant_and_type_conversion__1
   LDX   __local_test_non_constant_and_type_conversion__arr+47
   STX   __local_test_non_constant_and_type_conversion__0
   ORA   __local_test_non_constant_and_type_conversion__6
   ORA   __local_test_non_constant_and_type_conversion__5
   ORA   __local_test_non_constant_and_type_conversion__4
   ORA   __local_test_non_constant_and_type_conversion__3
   ORA   __local_test_non_constant_and_type_conversion__2
   ORA   __local_test_non_constant_and_type_conversion__1
   ORA   __local_test_non_constant_and_type_conversion__0
   BEQ   .if_end@21
.test_non_constant_and_type_conversion@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@21:
   JMP   test_simple@tail0

test_non_constant_and_type_conversion@run0:
   SUBROUTINE

   LDA   @9.x
   STA   HARGS
   LDA   @9.x+1
   STA   HARGS+1
   RTS

three:
   SUBROUTINE

   LDA   #$03
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

test_preserve_stack:
   SUBROUTINE

   LDA   one
   STA   __local_test_preserve_stack__arr_0
   LDA   one+1
   ASL   __local_test_preserve_stack__arr_0
   ROL
   STA   __local_test_preserve_stack__arr_1
   JSR   three
   LDA   one
   CLC
   ADC   HARGS+8
   STA   __local_test_preserve_stack__arr_2
   LDA   one+1
   ADC   HARGS+9
   STA   __local_test_preserve_stack__arr_3
   LDA   __local_test_preserve_stack__arr_1
   BNE   .test_preserve_stack@jthread@1
   LDA   __local_test_preserve_stack__arr_0
   CMP   #$02
   BNE   .test_preserve_stack@jthread@1
   LDA   __local_test_preserve_stack__arr_3
   BNE   .test_preserve_stack@jthread@1
   LDA   __local_test_preserve_stack__arr_2
   CMP   #$04
   BNE   .test_preserve_stack@jthread@1
   LDA   #$00
   BEQ   .if_end@28
.test_preserve_stack@jthread@1:
   JMP   test_simple@tail1
.if_end@28:
   JMP   test_simple@tail0

main:
   SUBROUTINE

   JSR   test_simple
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@29
   JMP   test_simple@tail0
.if_end@29:
   JSR   test_partial
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@30
   LDX   #$00
   LDA   #$02
   RTS
.if_end@30:
   JSR   test_non_constant_and_type_conversion
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@31
   LDX   #$00
   LDA   #$03
   RTS
.if_end@31:
   JSR   test_preserve_stack
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@32
   LDX   #$00
   LDA   #$04
   RTS
.if_end@32:
   JMP   test_simple@tail1

@9.x:
   DC.W  $0064

one:
   DC.L  $00000001
   DC.L  $00000000
