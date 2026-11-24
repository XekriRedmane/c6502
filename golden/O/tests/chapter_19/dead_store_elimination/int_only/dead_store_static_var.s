SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__arg_0,__zpabi_target__arg_1 locals=__local_target__arg_1,__local_target__arg_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item i section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   STA   i
   STX   i+1
   CLC
   ADC   #$01
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   JSR   target
   STA   __local_main__1
   LDA   i+1
   BNE   .jcmp_skip@0
   LDA   i
   CMP   #$02
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$FF
   TXA
   JSR   target
   STA   __local_main__1
   STX   __local_main__0
   LDA   i+1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   i
   CMP   #$FF
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

i:
   DS.B  2
