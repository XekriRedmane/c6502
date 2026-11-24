SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
called_counter	EQU	$26
__local_main__1	EQU	$81
__local_target__y	EQU	$82
__local_callee__i_0	EQU	$85

; @zp-link-meta-begin
; def callee params=__zpabi_callee__i_0,__zpabi_callee__i_1 locals=__local_callee__i_1,__local_callee__i_0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params= locals=__local_target__y,__local_target__0 indirect=false in_cycle=false
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp called_counter size=2 at=auto def=true init=$00,$00 weight=6 local=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   STA   __local_callee__i_0
   TXA
   BNE   .if_end@2
   LDA   __local_callee__i_0
   CMP   #$03
   BNE   .if_end@2
   LDA   called_counter
   ORA   called_counter+1
   BNE   .if_end@5
   LDA   #$01
   STA   called_counter
   LDA   #$00
   STA   called_counter+1
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   TXA
   BNE   .if_end@5
   LDA   __local_callee__i_0
   CMP   #$04
   BNE   .if_end@5
   LDA   called_counter+1
   BNE   .if_end@5
   LDA   called_counter
   CMP   #$01
   BNE   .if_end@5
   LDA   #$02
   STA   called_counter
   LDA   #$00
   STA   called_counter+1
   LDX   #$00
   RTS
.if_end@5:
   LDA   #$FF
   STA   called_counter
   STA   called_counter+1
   LDX   #$00
   TXA
   RTS

target:
   SUBROUTINE

   LDA   #$03
   LDX   #$00
.loop@0_start:
   JSR   callee
   STX   __local_target__y
   ORA   __local_target__y
   BNE   .target@asm_ssa_split@0
   LDX   #$00
   LDA   #$04
   RTS
.target@asm_ssa_split@0:
   LDA   #$04
   LDX   #$00
   BEQ   .loop@0_start

main:
   SUBROUTINE

   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$04
   BEQ   .if_end@6
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@6:
   LDA   called_counter+1
   BNE   .jcmp_skip@1
   LDA   called_counter
   CMP   #$02
   BEQ   .if_end@7
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   called_counter
   STA   called_counter+1
   RTS
