SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
@3.sc	EQU	$26
@2.uc	EQU	$27
@0.c	EQU	$28

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp @3.sc size=1 at=auto def=true init=$46 weight=7 local=true
; zp @2.uc size=1 at=auto def=true init=$C8 weight=6 local=true
; zp @0.c size=1 at=auto def=true init=$64 weight=5 local=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   @0.c
   CLC
   ADC   #$64
   STA   @0.c
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   @0.c
   CMP   #$C8
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   @2.uc
   LDX   #$FF
   SEC
.div_loop@1:
   INX
   SBC   #$9C
   BCS   .div_loop@1
   STX   @2.uc
   TXA
   CMP   #$FE
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   u2d
   LDA   HARGS+2
   STA   HARGS
   LDA   HARGS+3
   STA   HARGS+1
   LDA   HARGS+4
   STA   HARGS+2
   LDA   HARGS+5
   STA   HARGS+3
   LDA   HARGS+6
   STA   HARGS+4
   LDA   HARGS+7
   STA   HARGS+5
   LDA   HARGS+8
   STA   HARGS+6
   LDA   HARGS+9
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STA   HARGS+12
   LDA   #$40
   STA   HARGS+13
   LDY   #$6F
   STY   HARGS+14
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   STA   HARGS+4
   LDA   HARGS+21
   STA   HARGS+5
   LDA   HARGS+22
   STA   HARGS+6
   LDA   HARGS+23
   STA   HARGS+7
   JSR   d2u
   LDA   HARGS+8
   STA   @2.uc
   CMP   #$04
   BEQ   .vthread@0
   LDX   #$00
   LDA   #$03
   RTS
.vthread@0:
   SEC
   LDA   #$00
   SBC   @3.sc
   STA   @3.sc
   STA   HARGS
   LDA   @0.c
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   @3.sc
   CMP   #$50
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   HARGS+1
   LDA   @3.sc
   STA   HARGS
   LDA   @0.c
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   @3.sc
   CMP   #$18
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$64
   STA   @0.c
   LDA   #$C8
   STA   @2.uc
   LDA   #$46
   STA   @3.sc
   RTS
