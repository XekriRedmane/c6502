SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_add params= locals= indirect=false in_cycle=false
; def target_complement params= locals= indirect=false in_cycle=false
; def target_div params= locals= indirect=false in_cycle=false
; def target_eq params= locals= indirect=false in_cycle=false
; def target_ge params= locals= indirect=false in_cycle=false
; def target_gt params= locals= indirect=false in_cycle=false
; def target_le params= locals=__local_target_le__0 indirect=false in_cycle=false
; def target_lt params= locals= indirect=false in_cycle=false
; def target_mult params= locals= indirect=false in_cycle=false
; def target_neg params= locals= indirect=false in_cycle=false
; def target_neq params= locals= indirect=false in_cycle=false
; def target_not params= locals= indirect=false in_cycle=false
; def target_rem params= locals= indirect=false in_cycle=false
; def target_sub params= locals= indirect=false in_cycle=false
; call main -> target_add
; call main -> target_complement
; call main -> target_div
; call main -> target_eq
; call main -> target_ge
; call main -> target_gt
; call main -> target_le
; call main -> target_lt
; call main -> target_mult
; call main -> target_neg
; call main -> target_neq
; call main -> target_not
; call main -> target_rem
; call main -> target_sub
; item target_add section=.text local=false zero=false
; item target_sub section=.text local=false zero=false
; item target_mult section=.text local=false zero=false
; item target_mult@tail1 section=.text local=true zero=false
; item target_div section=.text local=false zero=false
; item target_rem section=.text local=false zero=false
; item target_complement section=.text local=false zero=false
; item target_neg section=.text local=false zero=false
; item target_not section=.text local=false zero=false
; item target_eq section=.text local=false zero=false
; item target_eq@tail0 section=.text local=true zero=false
; item target_neq section=.text local=false zero=false
; item target_gt section=.text local=false zero=false
; item target_ge section=.text local=false zero=false
; item target_lt section=.text local=false zero=false
; item target_le section=.text local=false zero=false
; item target_le@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item sub_result section=.data local=false zero=false
; item complement_result section=.data local=false zero=false
; item neg_result section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target_add:
   SUBROUTINE

   LDA   #$E7
   STA   HARGS+8
   LDA   #$03
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   LDA   #$80
   STA   HARGS+11
   RTS

target_sub:
   SUBROUTINE

   LDA   #$B9
   STA   HARGS+8
   LDA   #$0B
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11
   RTS

target_mult:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   JMP   target_mult@tail1

target_mult@tail1:
   SUBROUTINE

   STY   HARGS+10
   STA   HARGS+11
   RTS

target_div:
   SUBROUTINE

   LDA   #$92
   STA   HARGS+8
   LDA   #$49
   STA   HARGS+9
   LDA   #$AA
   STA   HARGS+10
   LDA   #$AE
   STA   HARGS+11
   RTS

target_rem:
   SUBROUTINE

   LDA   #$91
   STA   HARGS+8
   LDA   #$15
   STA   HARGS+9
   LDA   #$C9
   STA   HARGS+10
   LDA   #$68
   STA   HARGS+11
   RTS

target_complement:
   SUBROUTINE

   LDA   #$A9
   STA   HARGS+8
   LDA   #$AA
   STA   HARGS+9
   LDY   #$AA
   JMP   target_mult@tail1

target_neg:
   SUBROUTINE

   LDA   #$5C
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   JMP   target_mult@tail1

target_not:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_eq:
   SUBROUTINE

   JMP   target_eq@tail0

target_eq@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_neq:
   SUBROUTINE

   JMP   target_eq@tail0

target_gt:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_ge:
   SUBROUTINE

   JMP   target_eq@tail0

target_lt:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_le:
   SUBROUTINE

   LDA   #$00
   SEC
   SBC   #$80
   JSR   target_le@run0
   JSR   target_le@run0
   LDA   #$00
   ROL
   LDX   #$00
   RTS

target_le@run0:
   SUBROUTINE

   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   RTS

main:
   SUBROUTINE

   JSR   target_add
   LDA   HARGS+11
   BMI   .jcmp_skip@2
   CMP   #$80
   BNE   .jcmp_skip@2
   LDA   HARGS+10
   BNE   .jcmp_skip@2
   LDA   HARGS+9
   CMP   #$03
   BNE   .jcmp_skip@2
   LDA   HARGS+8
   CMP   #$E7
   BEQ   .if_end@0
.jcmp_skip@2:
   JMP   target_eq@tail0
.if_end@0:
   JSR   target_sub
   SEC
   LDA   #$00
   SBC   sub_result
   STA   __local_main__3
   LDA   #$00
   SBC   sub_result+1
   STA   __local_main__2
   LDA   #$00
   SBC   sub_result+2
   STA   __local_main__1
   LDA   #$00
   SBC   sub_result+3
   STA   __local_main__0
   LDA   HARGS+11
   CMP   __local_main__0
   BNE   .jcmp_skip@3
   LDA   HARGS+10
   CMP   __local_main__1
   BNE   .jcmp_skip@3
   LDA   HARGS+9
   CMP   __local_main__2
   BNE   .jcmp_skip@3
   LDA   HARGS+8
   CMP   __local_main__3
   BEQ   .if_end@1
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_mult
   LDX   #$00
   LDA   #$03
   RTS

sub_result:
   DC.L  $FFFFF447

complement_result:
   DC.L  $55555557

neg_result:
   DC.L  $FFFFFFA4
