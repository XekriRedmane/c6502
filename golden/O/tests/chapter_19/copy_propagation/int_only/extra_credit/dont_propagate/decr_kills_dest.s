SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_target__z_1	EQU	$82
__local_target__z_0	EQU	$83
__local_target__y_0	EQU	$85
__local_target__x	EQU	$86
__local_target__w	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__z_1,__local_target__z_0,__local_target__y_1,__local_target__y_0,__local_target__x,__local_target__w indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   STA   __local_target__z_0
   STX   __local_target__z_1
   ORA   __local_target__z_1
   BNE   .lb_skip@2
   JMP   .target@asm_ssa_split@0
.lb_skip@2:
   LDA   #$04
   STA   __local_target__w
.if_end@0:
   LDA   __local_target__z_0
   ORA   __local_target__z_1
   BNE   .lb_skip@1
   JMP   .target@asm_ssa_split@1
.lb_skip@1:
   LDA   #$09
   STA   __local_target__x
.if_end@1:
   LDA   __local_target__z_0
   ORA   __local_target__z_1
   BNE   .lb_skip@0
   JMP   .target@asm_ssa_split@2
.lb_skip@0:
   LDA   #$F5
   STA   __local_target__y_0
.if_end@2:
   LDA   __local_target__z_0
   ORA   __local_target__z_1
   BEQ   .target@asm_ssa_split@3
   LDA   __local_target__z_0
   ORA   __local_target__z_1
   BEQ   .target@asm_ssa_split@4
   LDA   __local_target__w
   CMP   #$04
   BNE   .if_end@19
   LDA   __local_target__x
   CMP   #$09
   BNE   .if_end@19
   LDA   #$FF
   CMP   #$FF
   BNE   .if_end@19
   LDA   __local_target__y_0
   CMP   #$F5
   BNE   .if_end@19
   LDA   #$FF
   CMP   #$FF
   BNE   .if_end@19
   LDA   #$9B
   CMP   #$9B
   BNE   .if_end@19
   LDX   #$00
   TXA
   RTS
.if_else@5:
   LDA   __local_target__w
   CMP   #$03
   BNE   .if_end@19
   LDA   __local_target__x
   CMP   #$0A
   BNE   .if_end@19
   LDA   #$FF
   CMP   #$FF
   BNE   .if_end@19
   LDA   __local_target__y_0
   CMP   #$F4
   BNE   .if_end@19
   LDA   #$FF
   CMP   #$FF
   BNE   .if_end@19
   LDA   __local_target__z_0
   CMP   #$9C
   BNE   .if_end@19
   LDX   #$00
   TXA
   RTS
.if_end@19:
   LDX   #$00
   LDA   #$01
   RTS
.target@asm_ssa_split@0:
   LDA   #$03
   STA   __local_target__w
   JMP   .if_end@0
.target@asm_ssa_split@1:
   LDA   #$0A
   STA   __local_target__x
   JMP   .if_end@1
.target@asm_ssa_split@2:
   LDA   #$F4
   STA   __local_target__y_0
   JMP   .if_end@2
.target@asm_ssa_split@3:
   LDA   #$9C
   STA   __local_target__z_0
   JMP   .if_else@5
.target@asm_ssa_split@4:
   LDA   #$9B
   STA   __local_target__z_0
   JMP   .if_else@5

main:
   SUBROUTINE

   LDX   #$00
   TXA
   JSR   target
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@20
   LDX   #$00
   LDA   #$01
   RTS
.if_end@20:
   LDX   #$00
   LDA   #$01
   JSR   target
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@21
   LDX   #$00
   LDA   #$02
   RTS
.if_end@21:
   LDX   #$00
   TXA
   RTS
