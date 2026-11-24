SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item target@run0 section=.text local=true zero=false
; item target@tail0 section=.text local=true zero=false
; item target@tail0@tail1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   JSR   target@run0
   STA   HARGS+18
   LDA   #$80
   STA   HARGS+19
   LDA   #$FF
   STA   HARGS+20
   LDY   #$FF
   JMP   target@tail0

target@run0:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   RTS

target@tail0:
   SUBROUTINE

   JMP   target@tail0@tail1

target@tail0@tail1:
   SUBROUTINE

   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   target
   LDA   HARGS+16
   EOR   #$FF
   STA   __local_main__7
   LDA   HARGS+17
   EOR   #$FF
   STA   __local_main__6
   LDA   HARGS+18
   EOR   #$FF
   STA   __local_main__5
   LDA   HARGS+19
   EOR   #$FF
   STA   __local_main__4
   LDA   HARGS+20
   EOR   #$FF
   STA   __local_main__3
   LDA   HARGS+21
   EOR   #$FF
   STA   __local_main__2
   LDA   HARGS+22
   EOR   #$FF
   STA   __local_main__1
   LDA   HARGS+23
   EOR   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   BNE   .jcmp_skip@0
   LDA   __local_main__2
   BNE   .jcmp_skip@0
   LDA   __local_main__3
   BNE   .jcmp_skip@0
   LDA   __local_main__4
   CMP   #$7F
   BNE   .jcmp_skip@0
   LDA   __local_main__5
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__6
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__7
   CMP   #$FF
   BEQ   .if_end@0
.jcmp_skip@0:
   LDA   #$01
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   JMP   target@tail0
.if_end@0:
   JSR   target@run0
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   JMP   target@tail0@tail1
