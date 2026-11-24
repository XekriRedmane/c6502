SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_fib__count_0	EQU	$80
__zpabi_putchar__c_0	EQU	$80
__zpabi_fib__count_1	EQU	$81
__zpabi_putchar__c_1	EQU	$81
__zpabi_fib__count_2	EQU	$82
__zpabi_putchar__c_2	EQU	$82
__zpabi_fib__count_3	EQU	$83
__zpabi_putchar__c_3	EQU	$83
__zpabi_fib__count_4	EQU	$84
__zpabi_putchar__c_4	EQU	$84
__zpabi_fib__count_5	EQU	$85
__zpabi_putchar__c_5	EQU	$85
__zpabi_fib__count_6	EQU	$86
__zpabi_putchar__c_6	EQU	$86
__zpabi_fib__count_7	EQU	$87
__zpabi_putchar__c_7	EQU	$87
__local_main__0	EQU	$88
__local_fib__n0_7	EQU	$90
__local_fib__n0_6	EQU	$91
__local_fib__n0_5	EQU	$92
__local_fib__n0_4	EQU	$93
__local_fib__n0_3	EQU	$94
__local_fib__n0_2	EQU	$95
__local_fib__n0_1	EQU	$96
__local_fib__n0_0	EQU	$97
__local_fib__i_7	EQU	$98
__local_fib__i_6	EQU	$99
__local_fib__i_5	EQU	$9A
__local_fib__i_4	EQU	$9B
__local_fib__i_3	EQU	$9C
__local_fib__i_0	EQU	$9D
__local_fib__n1_7	EQU	$9E
__local_fib__n1_6	EQU	$9F
__local_fib__n1_5	EQU	$A0
__local_fib__n1_4	EQU	$A1
__local_fib__n1_3	EQU	$A2
__local_fib__n1_2	EQU	$A3
__local_fib__n1_1	EQU	$A4
__local_fib__n1_0	EQU	$A5

; @zp-link-meta-begin
; def fib params=__zpabi_fib__count_0,__zpabi_fib__count_1,__zpabi_fib__count_2,__zpabi_fib__count_3,__zpabi_fib__count_4,__zpabi_fib__count_5,__zpabi_fib__count_6,__zpabi_fib__count_7 locals=__local_fib__n0_7,__local_fib__n0_6,__local_fib__n0_5,__local_fib__n0_4,__local_fib__n0_3,__local_fib__n0_2,__local_fib__n0_1,__local_fib__n0_0,__local_fib__i_7,__local_fib__i_6,__local_fib__i_5,__local_fib__i_4,__local_fib__i_3,__local_fib__i_0,__local_fib__n1_7,__local_fib__n1_6,__local_fib__n1_5,__local_fib__n1_4,__local_fib__n1_3,__local_fib__n1_2,__local_fib__n1_1,__local_fib__n1_0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1,__zpabi_putchar__c_2,__zpabi_putchar__c_3,__zpabi_putchar__c_4,__zpabi_putchar__c_5,__zpabi_putchar__c_6,__zpabi_putchar__c_7
; call main -> fib
; item fib section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

fib:
   SUBROUTINE

   ; prologue: 0 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   STA   __local_fib__n1_0
   LDA   #$00
   STA   __local_fib__n0_0
   STA   __local_fib__n0_1
   STA   __local_fib__n0_2
   STA   __local_fib__n0_3
   STA   __local_fib__n0_4
   STA   __local_fib__n0_5
   STA   __local_fib__n0_6
   STA   __local_fib__n0_7
   STA   __local_fib__n1_1
   STA   __local_fib__n1_2
   STA   __local_fib__n1_3
   STA   __local_fib__n1_4
   STA   __local_fib__n1_5
   STA   __local_fib__n1_6
   STA   __local_fib__n1_7
   STA   __local_fib__i_0
   LDX   #$00
   LDY   #$00
   STA   __local_fib__i_3
   STA   __local_fib__i_4
   STA   __local_fib__i_5
   STA   __local_fib__i_6
   STA   __local_fib__i_7
.loop@0_start:
   LDA   __local_fib__n0_0
   CLC
   ADC   __local_fib__n1_0
   STA   __local_fib__n0_0
   LDA   __local_fib__n0_1
   ADC   __local_fib__n1_1
   STA   __local_fib__n0_1
   LDA   __local_fib__n0_2
   ADC   __local_fib__n1_2
   STA   __local_fib__n0_2
   LDA   __local_fib__n0_3
   ADC   __local_fib__n1_3
   STA   __local_fib__n0_3
   LDA   __local_fib__n0_4
   ADC   __local_fib__n1_4
   STA   __local_fib__n0_4
   LDA   __local_fib__n0_5
   ADC   __local_fib__n1_5
   STA   __local_fib__n0_5
   LDA   __local_fib__n0_6
   ADC   __local_fib__n1_6
   STA   __local_fib__n0_6
   LDA   __local_fib__n0_7
   ADC   __local_fib__n1_7
   STA   __local_fib__n0_7
   INC   __local_fib__i_0
   BNE   .inc_done@0
   INX
   BNE   .inc_done@0
   INY
   BNE   .inc_done@0
   INC   __local_fib__i_3
   BNE   .inc_done@0
   INC   __local_fib__i_4
   BNE   .inc_done@0
   INC   __local_fib__i_5
   BNE   .inc_done@0
   INC   __local_fib__i_6
   BNE   .inc_done@0
   INC   __local_fib__i_7
.inc_done@0:
   LDA   __local_fib__i_0
   SEC
   SBC   __zpabi_fib__count_0
   TXA
   SBC   __zpabi_fib__count_1
   TYA
   SBC   __zpabi_fib__count_2
   LDA   __local_fib__i_3
   SBC   __zpabi_fib__count_3
   LDA   __local_fib__i_4
   SBC   __zpabi_fib__count_4
   LDA   __local_fib__i_5
   SBC   __zpabi_fib__count_5
   LDA   __local_fib__i_6
   SBC   __zpabi_fib__count_6
   LDA   __local_fib__i_7
   SBC   __zpabi_fib__count_7
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .fib@asm_ssa_split@0
   LDA   __local_fib__n0_0
   STA   HARGS+16
   LDA   __local_fib__n0_1
   STA   HARGS+17
   LDA   __local_fib__n0_2
   STA   HARGS+18
   LDA   __local_fib__n0_3
   STA   HARGS+19
   LDA   __local_fib__n0_4
   STA   HARGS+20
   LDA   __local_fib__n0_5
   STA   HARGS+21
   LDA   __local_fib__n0_6
   STA   HARGS+22
   LDA   __local_fib__n0_7
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.fib@asm_ssa_split@0:
   LDA   __local_fib__n0_0
   LDY   #$01
   STA   (FP),Y
   LDA   __local_fib__n1_0
   STA   __local_fib__n0_0
   LDA   (FP),Y
   STA   __local_fib__n1_0
   LDA   __local_fib__n0_1
   INY
   STA   (FP),Y
   LDA   __local_fib__n1_1
   STA   __local_fib__n0_1
   LDA   (FP),Y
   STA   __local_fib__n1_1
   LDA   __local_fib__n0_2
   INY
   STA   (FP),Y
   LDA   __local_fib__n1_2
   STA   __local_fib__n0_2
   LDA   (FP),Y
   STA   __local_fib__n1_2
   LDA   __local_fib__n0_3
   INY
   STA   (FP),Y
   LDA   __local_fib__n1_3
   STA   __local_fib__n0_3
   LDA   (FP),Y
   STA   __local_fib__n1_3
   LDA   __local_fib__n0_4
   INY
   STA   (FP),Y
   LDA   __local_fib__n1_4
   STA   __local_fib__n0_4
   LDA   (FP),Y
   STA   __local_fib__n1_4
   LDA   __local_fib__n0_5
   INY
   STA   (FP),Y
   LDA   __local_fib__n1_5
   STA   __local_fib__n0_5
   LDA   (FP),Y
   STA   __local_fib__n1_5
   LDA   __local_fib__n0_6
   INY
   STA   (FP),Y
   LDA   __local_fib__n1_6
   STA   __local_fib__n0_6
   LDA   (FP),Y
   STA   __local_fib__n1_6
   LDA   __local_fib__n0_7
   INY
   STA   (FP),Y
   LDA   __local_fib__n1_7
   STA   __local_fib__n0_7
   LDA   (FP),Y
   STA   __local_fib__n1_7
   JMP   .loop@0_start

main:
   SUBROUTINE

   LDA   #$14
   STA   __zpabi_fib__count_0
   LDA   #$00
   STA   __zpabi_fib__count_1
   STA   __zpabi_fib__count_2
   STA   __zpabi_fib__count_3
   STA   __zpabi_fib__count_4
   STA   __zpabi_fib__count_5
   STA   __zpabi_fib__count_6
   STA   __zpabi_fib__count_7
   JSR   fib
   LDA   HARGS+23
   BNE   .thread@0
   LDA   HARGS+22
   BNE   .thread@0
   LDA   HARGS+21
   BNE   .thread@0
   LDA   HARGS+20
   BNE   .thread@0
   LDA   HARGS+19
   BNE   .thread@0
   LDA   HARGS+18
   BNE   .thread@0
   LDA   HARGS+17
   CMP   #$2A
   BNE   .thread@0
   LDA   HARGS+16
   CMP   #$C2
   BEQ   .cmp_true@1
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   STA   __local_main__0
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_main__0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS
