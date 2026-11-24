SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__arr_10	EQU	$80
__local_main__arr_11	EQU	$81
__local_main__arr_1	EQU	$82
__local_main__arr_0	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__arr_10,__local_main__arr_11,__local_main__arr_1,__local_main__arr_0,__local_main__arr_9,__local_main__arr_8,__local_main__arr_7,__local_main__arr_6,__local_main__arr_5,__local_main__arr_4,__local_main__arr_3,__local_main__arr_2 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$0C
   STA   __local_main__arr_10
   LDA   #$00
   STA   __local_main__arr_11
   LDY   #$04
   ORA   #$00
   BNE   .jcmp_skip@0
   CPY   #$04
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   #$F5
   EOR   #$0B
   STA   __local_main__arr_0
   LDA   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_main__arr_0
   CMP   #$FE
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$78
   STA   __local_main__arr_1
   LDA   #$00
   STA   __local_main__arr_0
   LDX   #$03
.shift_loop@3:
   LDA   __local_main__arr_0
   ASL
   ROR   __local_main__arr_0
   ROR   __local_main__arr_1
   DEX
   BNE   .shift_loop@3
   LDA   __local_main__arr_0
   BNE   .jcmp_skip@5
   LDA   __local_main__arr_1
   CMP   #$0F
   BEQ   .if_end@3
.jcmp_skip@5:
   LDX   #$00
   TYA
   RTS
.if_end@3:
   ASL   __local_main__arr_10
   ROL   __local_main__arr_11
   ASL   __local_main__arr_10
   ROL   __local_main__arr_11
   ASL   __local_main__arr_10
   ROL   __local_main__arr_11
   BNE   .jcmp_skip@6
   LDA   __local_main__arr_10
   CMP   #$60
   BEQ   .if_end@4
.jcmp_skip@6:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS
