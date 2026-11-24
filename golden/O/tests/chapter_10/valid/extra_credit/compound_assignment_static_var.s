SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def f params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.i section=.bss local=true zero=true
; item @1.j section=.bss local=true zero=true
; item @2.k section=.data local=true zero=false
; item @3.l section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

f:
   SUBROUTINE

   INC   @0.i
   BNE   .inc_done@0
   INC   @0.i+1
.inc_done@0:
   LDA   @1.j
   SEC
   SBC   @0.i
   STA   @1.j
   LDA   @1.j+1
   SBC   @0.i+1
   STA   @1.j+1
   LDA   @2.k
   STA   HARGS
   LDA   @2.k+1
   STA   HARGS+1
   LDA   @1.j
   STA   HARGS+2
   LDA   @1.j+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   @2.k
   LDA   HARGS+5
   STA   @2.k+1
   LDA   @3.l
   STA   HARGS
   LDA   @3.l+1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   @3.l
   LDA   HARGS+5
   STA   @3.l+1
   LDA   @0.i+1
   BNE   .jcmp_skip@0
   LDA   @0.i
   CMP   #$03
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   @1.j+1
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   @1.j
   CMP   #$FA
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   @2.k+1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   @2.k
   CMP   #$EE
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   @3.l+1
   BNE   .jcmp_skip@3
   LDA   @3.l
   CMP   #$06
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   f
   JSR   f
   JMP   f

@0.i:
   DS.B  2

@1.j:
   DS.B  2

@2.k:
   DC.W  $0001

@3.l:
   DC.W  $0030
