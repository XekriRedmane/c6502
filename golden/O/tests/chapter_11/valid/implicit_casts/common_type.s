SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
i	EQU	$26
__local_main__0	EQU	$80
__local_addition__0	EQU	$82
__local_comparison__0	EQU	$82
__local_addition__1	EQU	$83
__local_comparison__1	EQU	$83
__local_addition__2	EQU	$84
__local_comparison__2	EQU	$84
__local_addition__3	EQU	$85
__local_comparison__3	EQU	$85
__local_addition__4	EQU	$86
__local_comparison__4	EQU	$86
__local_addition__5	EQU	$87
__local_comparison__5	EQU	$87
__local_addition__6	EQU	$88
__local_addition__7	EQU	$89

; @zp-link-meta-begin
; def addition params= locals=__local_addition__0,__local_addition__1,__local_addition__2,__local_addition__3,__local_addition__4,__local_addition__5,__local_addition__6,__local_addition__7 indirect=false in_cycle=false
; def comparison params= locals=__local_comparison__0,__local_comparison__1,__local_comparison__2,__local_comparison__3,__local_comparison__4,__local_comparison__5,__local_comparison__6,__local_comparison__7 indirect=false in_cycle=false
; def conditional params= locals=__local_conditional__0,__local_conditional__1,__local_conditional__2,__local_conditional__3,__local_conditional__4,__local_conditional__5,__local_conditional__6,__local_conditional__7 indirect=false in_cycle=false
; def division params= locals=__local_division__0,__local_division__1,__local_division__2,__local_division__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> addition
; call main -> comparison
; call main -> conditional
; call main -> division
; item addition section=.text local=false zero=false
; item division section=.text local=false zero=false
; item comparison section=.text local=false zero=false
; item conditional section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; item l section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp i size=2 at=auto def=true init=$00,$00 weight=7
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

addition:
   SUBROUTINE

   LDA   i+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_addition__5
   STA   __local_addition__4
   STA   __local_addition__3
   STA   __local_addition__2
   STA   __local_addition__1
   STA   __local_addition__0
   LDA   i
   CLC
   ADC   l
   STA   __local_addition__7
   LDA   i+1
   ADC   l+1
   STA   __local_addition__6
   LDA   __local_addition__5
   ADC   l+2
   STA   __local_addition__5
   LDA   __local_addition__4
   ADC   l+3
   STA   __local_addition__4
   LDA   __local_addition__3
   ADC   l+4
   STA   __local_addition__3
   LDA   __local_addition__2
   ADC   l+5
   STA   __local_addition__2
   LDA   __local_addition__1
   ADC   l+6
   STA   __local_addition__1
   LDA   __local_addition__0
   ADC   l+7
   BNE   .thread@0
   LDA   __local_addition__1
   BNE   .thread@0
   LDA   __local_addition__2
   BNE   .thread@0
   LDA   __local_addition__3
   BNE   .thread@0
   LDA   __local_addition__4
   BNE   .thread@0
   LDA   __local_addition__5
   BNE   .thread@0
   LDA   __local_addition__6
   CMP   #$80
   BNE   .thread@0
   LDA   __local_addition__7
   CMP   #$8C
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDX   #$00
   RTS

division:
   SUBROUTINE

   LDA   i+1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   LDA   l
   STA   HARGS
   LDA   l+1
   STA   HARGS+1
   LDA   l+2
   STA   HARGS+2
   LDA   l+3
   STA   HARGS+3
   LDA   l+4
   STA   HARGS+4
   LDA   l+5
   STA   HARGS+5
   LDA   l+6
   STA   HARGS+6
   LDA   l+7
   STA   HARGS+7
   LDA   i
   STA   HARGS+8
   LDA   i+1
   STA   HARGS+9
   JSR   sdivmod64
   LDA   HARGS+19
   BNE   .thread@1
   LDA   HARGS+18
   BNE   .thread@1
   LDA   HARGS+17
   CMP   #$27
   BNE   .thread@1
   LDA   HARGS+16
   CMP   #$10
   BEQ   .cmp_true@7
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDX   #$00
   RTS

comparison:
   SUBROUTINE

   LDA   i+1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_comparison__5
   STA   __local_comparison__4
   STA   __local_comparison__3
   STA   __local_comparison__2
   STA   __local_comparison__1
   STA   __local_comparison__0
   LDA   l
   SEC
   SBC   i
   LDA   l+1
   SBC   i+1
   LDA   l+2
   SBC   __local_comparison__5
   LDA   l+3
   SBC   __local_comparison__4
   LDA   l+4
   SBC   __local_comparison__3
   LDA   l+5
   SBC   __local_comparison__2
   LDA   l+6
   SBC   __local_comparison__1
   LDA   l+7
   SBC   __local_comparison__0
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDX   #$00
   RTS

conditional:
   SUBROUTINE

   LDA   l+7
   BNE   .thread@2
   LDA   l+6
   BNE   .thread@2
   LDA   l+5
   BNE   .thread@2
   LDA   l+4
   BNE   .thread@2
   LDA   l+3
   CMP   #$40
   BNE   .thread@2
   LDA   l+2
   BNE   .thread@2
   LDA   l+1
   BNE   .thread@2
   LDA   l
   BEQ   .cmp_true@13
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$82
   STA   l
   LDA   #$80
   STA   l+1
   LDA   #$0A
   STA   i
   LDA   #$00
   STA   l+2
   STA   l+3
   JSR   main@run0
   STA   i+1
   JSR   addition
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDA   #$A0
   STA   l
   LDA   #$86
   STA   l+1
   LDA   #$01
   STA   l+2
   LDA   #$00
   STA   l+3
   JSR   main@run0
   JSR   division
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDA   #$9C
   STA   i
   LDA   #$FF
   STA   i+1
   LDA   #$82
   STA   l
   LDA   #$80
   STA   l+1
   LDA   #$00
   STA   l+2
   STA   l+3
   JSR   main@run0
   JSR   comparison
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$03
   RTS
.if_end@4:
   LDA   #$40
   STA   l+3
   LDA   #$0A
   STA   i
   LDA   #$00
   STA   l
   STA   l+1
   STA   l+2
   JSR   main@run0
   STA   i+1
   JSR   conditional
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$04
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   STA   l+4
   STA   l+5
   STA   l+6
   STA   l+7
   RTS

l:
   DS.B  8

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   i
   STA   i+1
   RTS
