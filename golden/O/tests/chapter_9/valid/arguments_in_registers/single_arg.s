SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_twice__x_0	EQU	$82
__local_twice__x_1	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def twice params=__zpabi_twice__x_0,__zpabi_twice__x_1 locals=__local_twice__x_0,__local_twice__x_1 indirect=false in_cycle=false param_regs=A,X
; call main -> twice
; item twice section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

twice:
   SUBROUTINE

   STA   __local_twice__x_0
   STX   __local_twice__x_1
   ASL   __local_twice__x_0
   ROL   __local_twice__x_1
   LDX   __local_twice__x_1
   LDA   __local_twice__x_0
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   JSR   twice
   STA   __local_main__1
   TXA
   BNE   .thread@0
   LDA   __local_main__1
   CMP   #$06
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
