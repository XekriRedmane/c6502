SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   STA   HARGS+9
   LDA   #$FC
   STA   HARGS+10
   LDA   #$FF
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   JSR   target
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+10
   CMP   #$FC
   BNE   .jcmp_skip@0
   LDA   HARGS+9
   BNE   .jcmp_skip@0
   LDA   HARGS+8
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS
