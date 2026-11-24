SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__z	EQU	$80
__local_main__y	EQU	$81
__local_main__x_1	EQU	$82
__local_main__x_0	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__z,__local_main__y,__local_main__x_1,__local_main__x_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__x_0
   LDA   #$00
   STA   __local_main__x_1
   STA   __local_main__y
   LDX   #$00
   STA   __local_main__z
   LDY   #$00
.loop@0_start:
   INC   __local_main__z
   BNE   .inc_done@0
   INY
.inc_done@0:
   LDA   __local_main__x_1
   BMI   .loop@0_continue
   ORA   __local_main__x_0
   BEQ   .loop@0_continue
   LDA   __local_main__x_0
   BNE   .carry_inc@0
   DEC   __local_main__x_1
.carry_inc@0:
   DEC   __local_main__x_0
   LDA   __local_main__y
   SEC
   SBC   #$0A
   TXA
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@0_continue
   INC   __local_main__y
   BNE   .inc_done@1
   INX
.inc_done@1:
.loop@0_continue:
   CPY   #$00
   BNE   .loop@0_start
   LDA   __local_main__z
   CMP   #$32
   BNE   .loop@0_start
   LDA   __local_main__x_0
   ORA   __local_main__x_1
   BNE   .and_false@2
   CPX   #$00
   BNE   .and_false@2
   LDA   __local_main__y
   CMP   #$0A
   BNE   .and_false@2
   LDA   #$01
   LDX   #$00
   RTS
.and_false@2:
   LDA   #$00
   LDX   #$00
   RTS
