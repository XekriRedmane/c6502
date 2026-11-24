SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83

; @zp-link-meta-begin
; def a params= locals= indirect=false in_cycle=false
; def b params=__zpabi_b__a_0,__zpabi_b__a_1 locals=__local_b__a indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; call main -> a
; call main -> b
; item a section=.text local=false zero=false
; item b section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

a:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

b:
   SUBROUTINE

   RTS

main:
   SUBROUTINE

   JSR   a
   STA   __local_main__3
   STX   __local_main__2
   LDX   #$00
   LDA   #$02
   JSR   b
   CLC
   ADC   __local_main__3
   STA   __local_main__1
   TXA
   ADC   __local_main__2
   BNE   .thread@0
   LDA   __local_main__1
   CMP   #$03
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
