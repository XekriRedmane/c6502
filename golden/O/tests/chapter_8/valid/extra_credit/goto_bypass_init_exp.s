SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i_1	EQU	$80
__local_main__i_0	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__i_1,__local_main__i_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__i_0
   STA   __local_main__i_1
   JMP   .main@target
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$0A
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
.main@target:
   LDA   __local_main__i_0
   ORA   __local_main__i_1
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   INC   __local_main__i_0
   BNE   .loop@0_start
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   TXA
   RTS
