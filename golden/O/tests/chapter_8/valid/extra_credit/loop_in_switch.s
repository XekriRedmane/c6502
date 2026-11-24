SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i	EQU	$80
__local_main__cond	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__i,__local_main__cond indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__cond
   LDX   #$00
   LDA   #$00
   STA   __local_main__i
   LDY   #$00
.loop@3_start:
   LDA   __local_main__i
   SEC
   SBC   #$05
   TYA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@3_break
   LDA   __local_main__cond
   SEC
   SBC   #$01
   STA   __local_main__cond
   BCS   .carry_inc@0
   DEX
.carry_inc@0:
   CPX   #$00
   BNE   .if_end@0
   CMP   #$08
   BEQ   .loop@3_break
.if_end@0:
   INC   __local_main__i
   BNE   .loop@3_start
   INY
   JMP   .loop@3_start
.loop@3_break:
   LDX   #$00
   LDA   #$7B
   RTS
