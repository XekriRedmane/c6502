SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__index_3	EQU	$80
__local_main__index_2	EQU	$81
__local_main__index_0	EQU	$82
__local_main__counter	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__index_3,__local_main__index_2,__local_main__index_0,__local_main__counter indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item u section=.data local=false zero=false
; item ul section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   u+1
   BNE   .jcmp_skip@0
   LDA   u
   CMP   #$06
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   ul+3
   BNE   .jcmp_skip@1
   LDA   ul+2
   BNE   .jcmp_skip@1
   LDA   ul+1
   BNE   .jcmp_skip@1
   LDA   ul
   CMP   #$04
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$00
   STA   __local_main__counter
   LDX   #$00
   LDA   #$0A
   STA   __local_main__index_0
   LDY   #$00
   LDA   #$00
   STA   __local_main__index_2
   STA   __local_main__index_3
   JMP   .loop@0_start
.main@rot@0:
   INC   __local_main__counter
   BNE   .inc_done@0
   INX
.inc_done@0:
   LDA   __local_main__index_0
   SEC
   SBC   #$01
   STA   __local_main__index_0
   BCS   .carry_inc@0
   CPY   #$01
   DEY
.carry_inc@0:
   LDA   __local_main__index_2
   SBC   #$00
   STA   __local_main__index_2
   BCS   .loop@0_start
   DEC   __local_main__index_3
.loop@0_start:
   LDA   __local_main__index_0
   SEC
   SBC   #$FF
   TYA
   SBC   #$FF
   LDA   __local_main__index_2
   SBC   #$00
   LDA   __local_main__index_3
   SBC   #$00
   BCC   .main@rot@0
   CPX   #$00
   BNE   .jcmp_skip@2
   LDA   __local_main__counter
   CMP   #$0B
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

u:
   DC.W  $0006

ul:
   DC.L  $00000004
