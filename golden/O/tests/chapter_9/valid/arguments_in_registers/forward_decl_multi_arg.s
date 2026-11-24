SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_foo__y_0	EQU	$80
__zpabi_foo__y_1	EQU	$81
__local_main__1	EQU	$83
__local_foo__x_0	EQU	$84

; @zp-link-meta-begin
; def foo params=__zpabi_foo__x_0,__zpabi_foo__x_1,__zpabi_foo__y_0,__zpabi_foo__y_1 locals=__local_foo__x_0,__local_foo__x_1 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> foo
; item main section=.text local=false zero=false
; item foo section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_foo__y_0
   LDA   #$00
   STA   __zpabi_foo__y_1
   LDX   #$00
   LDA   #$02
   JSR   foo
   STA   __local_main__1
   TXA
   BNE   .thread@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   EOR   #$01
   LDX   #$00
   RTS

foo:
   SUBROUTINE

   SEC
   SBC   __zpabi_foo__y_0
   STA   __local_foo__x_0
   TXA
   SBC   __zpabi_foo__y_1
   TAX
   LDA   __local_foo__x_0
   RTS
