SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_test_automatic_nested__expected	EQU	$82
__local_test_static_nested__expected	EQU	$82
__local_test_automatic_nested__0	EQU	$83
__local_test_static_nested__0	EQU	$83
__local_test_static_nested__j_1	EQU	$84
__local_test_automatic_nested__2	EQU	$85
__local_test_static_nested__i	EQU	$85
__local_test_automatic_nested__k_1	EQU	$86
__local_test_automatic_nested__j_1	EQU	$87
__local_test_automatic_nested__j_0	EQU	$88
__local_test_automatic_nested__i_1	EQU	$89
__local_test_automatic_nested__i_0	EQU	$8A
__local_test_automatic_nested__nested_auto	EQU	$8B

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_automatic params= locals=__local_test_automatic__aut_3,__local_test_automatic__aut_2,__local_test_automatic__aut_1,__local_test_automatic__aut_0 indirect=false in_cycle=false
; def test_automatic_nested params= locals=__local_test_automatic_nested__expected,__local_test_automatic_nested__0,__local_test_automatic_nested__1,__local_test_automatic_nested__2,__local_test_automatic_nested__k_1,__local_test_automatic_nested__j_1,__local_test_automatic_nested__j_0,__local_test_automatic_nested__i_1,__local_test_automatic_nested__i_0,__local_test_automatic_nested__nested_auto,__local_test_automatic_nested__4,__local_test_automatic_nested__5,__local_test_automatic_nested__6,__local_test_automatic_nested__7,__local_test_automatic_nested__8,__local_test_automatic_nested__9,__local_test_automatic_nested__10,__local_test_automatic_nested__11,__local_test_automatic_nested__12,__local_test_automatic_nested__13,__local_test_automatic_nested__14,__local_test_automatic_nested__15,__local_test_automatic_nested__16,__local_test_automatic_nested__17,__local_test_automatic_nested__18 indirect=false in_cycle=false
; def test_static params= locals=__local_test_static__0 indirect=false in_cycle=false
; def test_static_nested params= locals=__local_test_static_nested__expected,__local_test_static_nested__0,__local_test_static_nested__j_1,__local_test_static_nested__i indirect=false in_cycle=false
; call main -> test_automatic
; call main -> test_automatic_nested
; call main -> test_static
; call main -> test_static_nested
; item test_static section=.text local=false zero=false
; item test_static_nested section=.text local=false zero=false
; item test_static_nested@tail0 section=.text local=true zero=false
; item test_automatic section=.text local=false zero=false
; item test_automatic_nested section=.text local=false zero=false
; item main section=.text local=false zero=false
; item static_arr section=.data local=true zero=false
; item nested_static_arr section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_static:
   SUBROUTINE

   LDA   static_arr
   CMP   #$68
   BNE   .and_false@0
   LDA   static_arr+1
   CMP   #$69
   BNE   .and_false@0
   LDA   static_arr+2
   BNE   .and_false@0
   LDA   static_arr+3
   BNE   .and_false@0
   LDA   static_arr+4
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
   LDX   #$00
   RTS

test_static_nested:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_static_nested__i
   LDX   #$00
.loop@0_start:
   LDA   __local_test_static_nested__i
   SEC
   SBC   #$03
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDY   #$00
   LDA   #$00
   STA   __local_test_static_nested__j_1
.loop@1_start:
   TYA
   SEC
   SBC   #$04
   LDA   __local_test_static_nested__j_1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@1_break
   LDA   __local_test_static_nested__i
   ASL
   ASL
   STA   __local_test_static_nested__expected
   LDA   #<nested_static_arr
   CLC
   ADC   __local_test_static_nested__expected
   STA   __local_test_static_nested__expected
   LDA   #>nested_static_arr
   ADC   #$00
   STA   __local_test_static_nested__0
   LDA   (__local_test_static_nested__expected),Y
   STA   __local_test_static_nested__0
   CPX   #$00
   BNE   .and_false@12
   LDA   __local_test_static_nested__i
   CMP   #$01
   BNE   .and_false@12
   TYA
   ORA   __local_test_static_nested__j_1
   BNE   .test_static_nested@cthread@0
   LDA   #$62
   STA   __local_test_static_nested__expected
   JMP   .if_end@10
.test_static_nested@cthread@0:
   LDA   __local_test_static_nested__j_1
   BNE   .and_false@12
   CPY   #$01
   BEQ   .test_static_nested@ssa_block@8
.and_false@12:
   LDA   #$00
   STA   __local_test_static_nested__expected
   JMP   .if_end@14
.test_static_nested@ssa_block@8:
   LDA   #$63
   STA   __local_test_static_nested__expected
.if_end@14:
.if_end@10:
   LDA   __local_test_static_nested__0
   CMP   __local_test_static_nested__expected
   BEQ   .if_end@15
   LDX   #$00
   TXA
   RTS
.if_end@15:
   INY
   BNE   .loop@1_start
   INC   __local_test_static_nested__j_1
   JMP   .loop@1_start
.loop@1_break:
   INC   __local_test_static_nested__i
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INX
   JMP   .loop@0_start
.loop@0_break:
   JMP   test_static_nested@tail0

test_static_nested@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_automatic:
   SUBROUTINE

   LDA   #$61
   CMP   #$61
   BNE   .and_false@16
   LDA   #$62
   CMP   #$62
   BNE   .and_false@16
   LDA   #$00
   BNE   .and_false@16
   LDA   #$01
   LDX   #$00
   RTS
.and_false@16:
   LDA   #$00
   LDX   #$00
   RTS

test_automatic_nested:
   SUBROUTINE

   LDA   #$66
   STA   __local_test_automatic_nested__nested_auto
   LDA   #$6F
   STA   __local_test_automatic_nested__nested_auto+1
   STA   __local_test_automatic_nested__nested_auto+2
   LDA   #$00
   STA   __local_test_automatic_nested__nested_auto+3
   LDY   #$00
   STY   __local_test_automatic_nested__nested_auto+4
   STY   __local_test_automatic_nested__nested_auto+5
   STY   __local_test_automatic_nested__nested_auto+6
   STA   __local_test_automatic_nested__nested_auto+7
   LDY   #$78
   STY   __local_test_automatic_nested__nested_auto+8
   LDY   #$00
   STY   __local_test_automatic_nested__nested_auto+9
   STY   __local_test_automatic_nested__nested_auto+10
   STA   __local_test_automatic_nested__nested_auto+11
   LDA   #$79
   STA   __local_test_automatic_nested__nested_auto+12
   LDA   #$7A
   STA   __local_test_automatic_nested__nested_auto+13
   TYA
   STA   __local_test_automatic_nested__nested_auto+14
   STA   __local_test_automatic_nested__nested_auto+15
   STA   __local_test_automatic_nested__i_0
   STY   __local_test_automatic_nested__i_1
.loop@2_start:
   LDA   __local_test_automatic_nested__i_0
   SEC
   SBC   #$02
   LDA   __local_test_automatic_nested__i_1
   SBC   #$00
   BVC   .jcmp_novf@2
   EOR   #$80
.jcmp_novf@2:
   BMI   .lb_skip@7
   JMP   .loop@2_break
.lb_skip@7:
   LDA   #$00
   STA   __local_test_automatic_nested__j_0
   LDY   #$00
   STY   __local_test_automatic_nested__j_1
.loop@3_start:
   LDA   __local_test_automatic_nested__j_0
   SEC
   SBC   #$02
   LDA   __local_test_automatic_nested__j_1
   SBC   #$00
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BMI   .lb_skip@6
   JMP   .loop@3_break
.lb_skip@6:
   LDY   #$00
   LDA   #$00
   STA   __local_test_automatic_nested__k_1
.loop@4_start:
   TYA
   SEC
   SBC   #$04
   LDA   __local_test_automatic_nested__k_1
   SBC   #$00
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BMI   .lb_skip@5
   JMP   .loop@4_break
.lb_skip@5:
   LDA   #>__local_test_automatic_nested__nested_auto
   STA   __local_test_automatic_nested__2
   LDA   __local_test_automatic_nested__i_0
   ASL
   ASL
   ASL
   STA   __local_test_automatic_nested__expected
   LDA   #<__local_test_automatic_nested__nested_auto
   CLC
   ADC   __local_test_automatic_nested__expected
   STA   __local_test_automatic_nested__0
   BCC   .carry_inc@0
   INC   __local_test_automatic_nested__2
.carry_inc@0:
   LDA   __local_test_automatic_nested__j_0
   ASL
   ASL
   CLC
   ADC   __local_test_automatic_nested__0
   STA   __local_test_automatic_nested__expected
   LDA   __local_test_automatic_nested__2
   ADC   #$00
   STA   __local_test_automatic_nested__0
   LDA   (__local_test_automatic_nested__expected),Y
   STA   __local_test_automatic_nested__0
   LDA   __local_test_automatic_nested__i_0
   ORA   __local_test_automatic_nested__i_1
   BNE   .if_else@25
   LDA   __local_test_automatic_nested__j_0
   ORA   __local_test_automatic_nested__j_1
   BNE   .and_false@43
   TYA
   ORA   __local_test_automatic_nested__k_1
   BNE   .if_else@27
   LDA   #$66
   STA   __local_test_automatic_nested__expected
   JMP   .if_end@24
.if_else@27:
   LDA   __local_test_automatic_nested__k_1
   BNE   .jcmp_skip@6
   CPY   #$01
   BEQ   .test_automatic_nested@ssa_block@10
   CMP   #$00
   BNE   .jcmp_skip@6
   CPY   #$02
   BEQ   .test_automatic_nested@ssa_block@10
.jcmp_skip@6:
   LDA   #$00
   STA   __local_test_automatic_nested__expected
   JMP   .if_end@24
.test_automatic_nested@ssa_block@10:
   LDA   #$6F
   STA   __local_test_automatic_nested__expected
   JMP   .if_end@24
.if_else@25:
   LDA   __local_test_automatic_nested__i_1
   BNE   .and_false@43
   LDA   __local_test_automatic_nested__i_0
   CMP   #$01
   BNE   .and_false@43
   LDA   __local_test_automatic_nested__j_0
   ORA   __local_test_automatic_nested__j_1
   BNE   .test_automatic_nested@cthread@0
   TYA
   ORA   __local_test_automatic_nested__k_1
   BNE   .and_false@43
   LDA   #$78
   STA   __local_test_automatic_nested__expected
   JMP   .if_end@35
.test_automatic_nested@cthread@0:
   LDA   __local_test_automatic_nested__j_1
   BNE   .and_false@43
   LDA   __local_test_automatic_nested__j_0
   CMP   #$01
   BNE   .and_false@43
   TYA
   ORA   __local_test_automatic_nested__k_1
   BNE   .test_automatic_nested@cthread@1
   LDA   #$79
   STA   __local_test_automatic_nested__expected
   JMP   .if_end@41
.test_automatic_nested@cthread@1:
   LDA   __local_test_automatic_nested__k_1
   BNE   .and_false@43
   CPY   #$01
   BEQ   .test_automatic_nested@ssa_block@25
.and_false@43:
   LDA   #$00
   STA   __local_test_automatic_nested__expected
   JMP   .if_end@47
.test_automatic_nested@ssa_block@25:
   LDA   #$7A
   STA   __local_test_automatic_nested__expected
.if_end@47:
.if_end@41:
.if_end@35:
.if_end@24:
   LDA   __local_test_automatic_nested__0
   CMP   __local_test_automatic_nested__expected
   BEQ   .if_end@48
   LDX   #$00
   TXA
   RTS
.if_end@48:
   INY
   BEQ   .lb_skip@4
   JMP   .loop@4_start
.lb_skip@4:
   INC   __local_test_automatic_nested__k_1
   JMP   .loop@4_start
.loop@4_break:
   INC   __local_test_automatic_nested__j_0
   BEQ   .lb_skip@3
   JMP   .loop@3_start
.lb_skip@3:
   INC   __local_test_automatic_nested__j_1
   JMP   .loop@3_start
.loop@3_break:
   INC   __local_test_automatic_nested__i_0
   BEQ   .lb_skip@2
   JMP   .loop@2_start
.lb_skip@2:
   INC   __local_test_automatic_nested__i_1
   JMP   .loop@2_start
.loop@2_break:
   JMP   test_static_nested@tail0

main:
   SUBROUTINE

   JSR   test_static
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@49
   JMP   test_static_nested@tail0
.if_end@49:
   JSR   test_static_nested
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@50
   LDX   #$00
   LDA   #$02
   RTS
.if_end@50:
   JSR   test_automatic
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@51
   LDX   #$00
   LDA   #$03
   RTS
.if_end@51:
   JSR   test_automatic_nested
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@52
   LDX   #$00
   LDA   #$04
   RTS
.if_end@52:
   LDX   #$00
   TXA
   RTS

static_arr:
   DC.B  $68, $69, $00, $00, $00

nested_static_arr:
   DS.B  4
   DC.B  $62, $63, $00, $00
   DS.B  4
