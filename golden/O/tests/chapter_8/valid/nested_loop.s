SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__y_1	EQU	$80
__local_main__y_0	EQU	$81
__local_main__x_1	EQU	$82
__local_main__x_0	EQU	$83
__local_main__acc	EQU	$84

; @zp-link-meta-begin
; def main params= locals=__local_main__y_1,__local_main__y_0,__local_main__x_1,__local_main__x_0,__local_main__acc indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__acc
   LDX   #$00
   LDA   #$64
   STA   __local_main__x_0
   LDY   #$00
   STY   __local_main__x_1
.loop@0_continue:
   LDA   __local_main__x_0
   ORA   __local_main__x_1
   BEQ   .loop@0_break
   LDA   __local_main__x_0
   SEC
   SBC   #$0A
   STA   __local_main__x_0
   BCS   .carry_inc@0
   DEC   __local_main__x_1
.carry_inc@0:
   LDA   #$0A
   STA   __local_main__y_0
   STY   __local_main__y_1
.loop@1_continue:
   ORA   __local_main__y_1
   BEQ   .loop@0_continue
   INC   __local_main__acc
   BNE   .inc_done@0
   INX
.inc_done@0:
   LDA   __local_main__y_0
   SEC
   SBC   #$01
   STA   __local_main__y_0
   BCS   .loop@1_continue
   DEC   __local_main__y_1
   JMP   .loop@1_continue
.loop@0_break:
   CPX   #$00
   BNE   .and_false@0
   LDA   __local_main__acc
   CMP   #$64
   BNE   .and_false@0
   LDA   __local_main__x_0
   ORA   __local_main__x_1
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   TYA
   LDX   #$00
   RTS
