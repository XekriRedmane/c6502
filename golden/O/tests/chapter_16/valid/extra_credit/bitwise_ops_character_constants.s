SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item @1.c section=.data local=true zero=false
; item @2.ul section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=17
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$21
   ORA   @1.c
   STA   __local_main__0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__0
   CMP   #$A5
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   @2.ul
   AND   #$7E
   CMP   #$26
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   @2.ul
   STA   HARGS
   LDA   @2.ul+1
   STA   HARGS+1
   LDA   @2.ul+2
   STA   HARGS+2
   LDA   @2.ul+3
   STA   HARGS+3
   LDA   @2.ul+4
   STA   HARGS+4
   LDA   @2.ul+5
   STA   HARGS+5
   LDA   @2.ul+6
   STA   HARGS+6
   LDA   @2.ul+7
   STA   HARGS+7
   LDA   #$04
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+16
   BNE   .jcmp_skip@1
   LDA   HARGS+15
   BNE   .jcmp_skip@1
   LDA   HARGS+14
   BNE   .jcmp_skip@1
   LDA   HARGS+13
   BNE   .jcmp_skip@1
   LDA   HARGS+12
   CMP   #$08
   BNE   .jcmp_skip@1
   LDA   HARGS+11
   CMP   #$08
   BNE   .jcmp_skip@1
   LDA   HARGS+10
   CMP   #$02
   BNE   .jcmp_skip@1
   LDA   HARGS+9
   CMP   #$60
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

@1.c:
   DC.B  $84

@2.ul:
   DC.L  $80808026
   DC.L  $00000000
