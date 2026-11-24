SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__counter_1	EQU	$80
__local_main__counter_0	EQU	$81
__local_main__i_1	EQU	$82
__local_main__i_0	EQU	$83
__local_main__sum_1	EQU	$84
__local_main__sum_0	EQU	$85

; @zp-link-meta-begin
; def main params= locals=__local_main__counter_1,__local_main__counter_0,__local_main__i_1,__local_main__i_0,__local_main__sum_1,__local_main__sum_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   STA   __local_main__sum_0
   STA   __local_main__sum_1
   STA   __local_main__i_0
   STA   __local_main__i_1
   LDY   #$01
   LDA   (FP),Y
   STA   __local_main__counter_0
   INY
   LDA   (FP),Y
   STA   __local_main__counter_1
.loop@0_start:
   LDA   #$0A
   SEC
   SBC   __local_main__i_0
   LDA   #$00
   SBC   __local_main__i_1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .loop@0_break
   LDA   __local_main__i_0
   STA   HARGS
   LDA   __local_main__i_1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BEQ   .loop@0_continue
   INC   __local_main__sum_0
   BNE   .inc_done@0
   INC   __local_main__sum_1
.inc_done@0:
.loop@0_continue:
   LDA   __local_main__i_0
   CLC
   ADC   #$01
   STA   __local_main__counter_0
   LDA   __local_main__i_1
   ADC   #$00
   STA   __local_main__counter_1
   LDA   __local_main__i_0
   LDY   #$03
   STA   (FP),Y
   LDA   __local_main__counter_0
   STA   __local_main__i_0
   LDA   (FP),Y
   STA   __local_main__counter_0
   LDA   __local_main__i_1
   INY
   STA   (FP),Y
   LDA   __local_main__counter_1
   STA   __local_main__i_1
   LDA   (FP),Y
   STA   __local_main__counter_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_main__sum_1
   BNE   .and_false@1
   LDA   __local_main__sum_0
   CMP   #$05
   BNE   .and_false@1
   LDA   __local_main__counter_1
   BNE   .and_false@1
   LDA   __local_main__counter_0
   CMP   #$0A
   BNE   .and_false@1
   LDA   #$01
   LDX   #$00
   BEQ   .and_end@2
.and_false@1:
   LDA   #$00
   LDX   #$00
.and_end@2:

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
