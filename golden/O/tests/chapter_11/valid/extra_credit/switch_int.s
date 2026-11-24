SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_switch_on_int__i_0	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def switch_on_int params=__zpabi_switch_on_int__i_0,__zpabi_switch_on_int__i_1 locals=__local_switch_on_int__i_1,__local_switch_on_int__i_0 indirect=false in_cycle=false param_regs=A,X
; call main -> switch_on_int
; item switch_on_int section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

switch_on_int:
   SUBROUTINE

   STA   __local_switch_on_int__i_0
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_switch_on_int__i_0
   CMP   #$05
   BEQ   .case@1
.jcmp_skip@0:
   CPX   #$01
   BNE   .jcmp_skip@1
   LDA   __local_switch_on_int__i_0
   BEQ   .case@2
.jcmp_skip@1:
   CPX   #$01
   BNE   .default@4
   LDA   __local_switch_on_int__i_0
   CMP   #$FF
   BEQ   .case@3
   BNE   .default@4
.case@1:
   LDX   #$00
   TXA
   RTS
.case@2:
   LDX   #$00
   LDA   #$01
   RTS
.case@3:
   LDX   #$00
   LDA   #$02
   RTS
.default@4:
   LDX   #$00
   LDA   #$03
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$05
   JSR   switch_on_int
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   JSR   switch_on_int
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$FF
   TXA
   JSR   switch_on_int
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@2
.jcmp_skip@4:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$01
   LDA   #$00
   JSR   switch_on_int
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@5
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@5:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
