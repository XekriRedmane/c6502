SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__count_0	EQU	$80
__local_main__count_1	EQU	$81
__local_main__iterations_1	EQU	$82
__local_main__iterations_0	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__count_0,__local_main__count_1,__local_main__iterations_1,__local_main__iterations_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$25
   STA   __local_main__count_0
   LDA   #$08
   STA   __local_main__iterations_0
   LDA   #$00
   STA   __local_main__count_1
   STA   __local_main__iterations_1
   JMP   .case@5
.loop@2_start:
   LDA   __local_main__count_0
   BNE   .carry_inc@0
   DEC   __local_main__count_1
.carry_inc@0:
   DEC   __local_main__count_0
   BNE   .carry_inc@1
   DEC   __local_main__count_1
.carry_inc@1:
   DEC   __local_main__count_0
   BNE   .carry_inc@2
   DEC   __local_main__count_1
.carry_inc@2:
   DEC   __local_main__count_0
.case@5:
   LDA   __local_main__count_0
   BNE   .carry_inc@3
   DEC   __local_main__count_1
.carry_inc@3:
   DEC   __local_main__count_0
   BNE   .carry_inc@4
   DEC   __local_main__count_1
.carry_inc@4:
   DEC   __local_main__count_0
   LDA   __local_main__iterations_0
   SEC
   SBC   #$01
   STA   __local_main__iterations_0
   BCS   .carry_inc@5
   DEC   __local_main__iterations_1
.carry_inc@5:
   LDA   __local_main__iterations_1
   BMI   .jcmp_neg@0
   ORA   __local_main__iterations_0
   BNE   .loop@2_start
.jcmp_neg@0:
   LDA   __local_main__count_0
   ORA   __local_main__count_1
   BNE   .and_false@0
   LDA   __local_main__iterations_0
   ORA   __local_main__iterations_1
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
   LDX   #$00
   RTS
