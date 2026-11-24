SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__count	EQU	$80
__local_main__i_1	EQU	$81
__local_main__i_0	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__count,__local_main__i_1,__local_main__i_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$64
   STA   __local_main__i_1
   LDX   #$00
   LDA   #$00
   STA   __local_main__count
   LDY   #$00
.loop@0_continue:
   LDA   __local_main__i_1
   SEC
   SBC   #$01
   STA   __local_main__i_1
   BCS   .carry_inc@0
   DEX
.carry_inc@0:
   CPX   #$FF
   BNE   .jcmp_skip@0
   CMP   #$FF
   BEQ   .loop@0_break
.jcmp_skip@0:
   INC   __local_main__count
   BNE   .loop@0_continue
   INY
   JMP   .loop@0_continue
.loop@0_break:
   CPY   #$00
   BNE   .jcmp_skip@2
   LDA   __local_main__count
   CMP   #$64
   BNE   .jcmp_skip@2
   LDA   #$64
   STA   __local_main__i_0
   LDA   #$00
   STA   __local_main__i_1
   STA   __local_main__count
   LDX   #$00
.loop@1_continue:
   LDA   __local_main__i_0
   SEC
   SBC   #$01
   STA   __local_main__i_0
   BCS   .carry_inc@1
   DEC   __local_main__i_1
.carry_inc@1:
   ORA   __local_main__i_1
   BEQ   .loop@1_break
   INC   __local_main__count
   BNE   .loop@1_continue
   INX
   JMP   .loop@1_continue
.loop@1_break:
   CPX   #$00
   BNE   .jcmp_skip@2
   LDA   __local_main__count
   CMP   #$63
   BEQ   .if_end@1
.jcmp_skip@2:
   LDX   #$00
   TXA
   RTS
.if_end@1:
   LDX   #$00
   LDA   #$01
   RTS
