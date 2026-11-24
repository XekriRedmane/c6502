SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$72
   STA   HARGS+16
   LDA   #$47
   STA   HARGS+17
   LDA   #$01
   STA   HARGS+18
   LDA   #$00
   STA   HARGS+19
   LDY   #$00
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   target
   LDA   HARGS+23
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   CMP   #$01
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   CMP   #$47
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   CMP   #$72
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS
