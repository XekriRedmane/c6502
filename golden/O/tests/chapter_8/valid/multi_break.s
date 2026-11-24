SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__j	EQU	$80
__local_main__i_1	EQU	$81
__local_main__i_0	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__j,__local_main__i_1,__local_main__i_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__i_0
   STA   __local_main__i_1
.loop@0_continue:
   INC   __local_main__i_0
   BNE   .inc_done@0
   INC   __local_main__i_1
.inc_done@0:
   LDA   #$0A
   SEC
   SBC   __local_main__i_0
   LDA   #$00
   SBC   __local_main__i_1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_continue
   LDA   #$0A
   STA   __local_main__j
   LDX   #$00
.loop@1_continue:
   LDA   __local_main__j
   SEC
   SBC   #$01
   STA   __local_main__j
   BCS   .carry_inc@0
   DEX
.carry_inc@0:
   TXA
   BPL   .loop@1_continue
   CPX   #$FF
   BNE   .and_false@2
   LDA   __local_main__j
   CMP   #$FF
   BNE   .and_false@2
   LDA   __local_main__i_1
   BNE   .and_false@2
   LDA   __local_main__i_0
   CMP   #$0B
   BNE   .and_false@2
   LDA   #$01
   LDX   #$00
   RTS
.and_false@2:
   LDA   #$00
   LDX   #$00
   RTS
