SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__x_1	EQU	$80
__local_main__x_0	EQU	$81

; @zp-link-meta-begin
; def foo params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__x_1,__local_main__x_0 indirect=false in_cycle=false
; call main -> foo
; item main section=.text local=false zero=false
; item foo section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   foo
   STA   __local_main__x_0
   STX   __local_main__x_1
   TXA
   BMI   .if_end@0
   ORA   __local_main__x_0
   BEQ   .if_end@0
   LDA   __local_main__x_0
   CLC
   ADC   #$03
   STA   __local_main__x_0
   BCC   .carry_inc@0
   INC   __local_main__x_1
.carry_inc@0:
.if_end@0:
   LDA   __local_main__x_1
   BNE   .thread@0
   LDA   __local_main__x_0
   CMP   #$07
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

   LDX   #$00
   LDA   #$04
   RTS
