SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__index	EQU	$80
__local_main__counter	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__index,__local_main__counter indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item i section=.data local=true zero=false
; item l section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   i+1
   BNE   .jcmp_skip@0
   LDA   i
   CMP   #$05
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   l+3
   BNE   .jcmp_skip@1
   LDA   l+2
   BNE   .jcmp_skip@1
   LDA   l+1
   BNE   .jcmp_skip@1
   LDA   l
   CMP   #$07
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
   STA   __local_main__index
   LDY   #$00
   BEQ   .loop@0_start
.main@rot@0:
   ORA   __local_main__index
   BEQ   .loop@0_break
   INC   __local_main__counter
   BNE   .inc_done@0
   INX
.inc_done@0:
   LDA   __local_main__index
   SEC
   SBC   #$01
   STA   __local_main__index
   BCS   .loop@0_start
   DEY
.loop@0_start:
   TYA
   BPL   .main@rot@0
.loop@0_break:
   CPX   #$00
   BNE   .jcmp_skip@2
   LDA   __local_main__counter
   CMP   #$0A
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

i:
   DC.W  $0005

l:
   DC.L  $00000007
