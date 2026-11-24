SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item target@tail0 section=.text local=true zero=false
; item target@tail0@tail1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$3C
   STA   HARGS+16
   LDA   #$F6
   STA   HARGS+17
   LDA   #$FF
   STA   HARGS+18
   LDY   #$FF
   JMP   target@tail0

target@tail0:
   SUBROUTINE

   JMP   target@tail0@tail1

target@tail0@tail1:
   SUBROUTINE

   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   target
   LDA   HARGS+23
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   CMP   #$F6
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   CMP   #$3C
   BEQ   .if_end@0
.jcmp_skip@0:
   LDA   #$01
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   JMP   target@tail0
.if_end@0:
   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   JMP   target@tail0@tail1
