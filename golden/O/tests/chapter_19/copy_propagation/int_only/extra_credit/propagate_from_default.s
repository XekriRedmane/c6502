SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
globvar	EQU	$26
__local_main__1	EQU	$81
__local_target__x_0	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__x_0,__zpabi_target__x_1 locals=__local_target__x_1,__local_target__x_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp globvar size=2 at=auto def=true init=$00,$00 weight=6
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   STA   __local_target__x_0
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_target__x_0
   CMP   #$01
   BEQ   .case@1
.jcmp_skip@0:
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_target__x_0
   CMP   #$02
   BEQ   .case@2
.jcmp_skip@1:
   TXA
   BNE   .default@4
   LDA   __local_target__x_0
   CMP   #$03
   BEQ   .case@3
   BNE   .default@4
.case@1:
   LDA   #$01
   STA   globvar
   LDY   #$00
   STY   globvar+1
.case@2:
   LDA   globvar
   CLC
   ADC   #$03
   STA   globvar
   BCC   .carry_inc@0
   INC   globvar+1
.carry_inc@0:
.case@3:
   ASL   globvar
   ROL   globvar+1
.default@4:
   LDX   #$00
   LDA   #$03
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@0
.jcmp_skip@3:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   globvar+1
   BNE   .jcmp_skip@4
   LDA   globvar
   CMP   #$06
   BEQ   .if_end@1
.jcmp_skip@4:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   globvar
   STA   globvar+1
   RTS
