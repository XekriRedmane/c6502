SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__y_0	EQU	$80
__zpabi_target__y_1	EQU	$81
__local_main__1	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__x_0,__zpabi_target__x_1,__zpabi_target__y_0,__zpabi_target__y_1 locals= indirect=false in_cycle=false param_regs=A,X,,
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_target__y_0
   LDA   #$00
   STA   __zpabi_target__y_1
   LDX   #$00
   LDA   #$01
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDA   #$00
   STA   __zpabi_target__y_0
   STA   __zpabi_target__y_1
   LDX   #$00
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
