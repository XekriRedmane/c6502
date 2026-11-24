SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_f__b_0	EQU	$80
__zpabi_f__b_1	EQU	$81
__zpabi_f__c_0	EQU	$82
__zpabi_f__c_1	EQU	$83
__zpabi_f__d_0	EQU	$84
__zpabi_f__d_1	EQU	$85
__zpabi_g__x_0	EQU	$86
__zpabi_g__x_1	EQU	$87
__zpabi_g__y_0	EQU	$88
__zpabi_g__y_1	EQU	$89
__zpabi_g__z_0	EQU	$8A
__zpabi_g__z_1	EQU	$8B
__local_main__1	EQU	$8D
__local_f__6	EQU	$94
__local_f__7	EQU	$95
__local_f__a_1	EQU	$96
__local_f__a_0	EQU	$97
__local_g__w_0	EQU	$99

; @zp-link-meta-begin
; def f params=__zpabi_f__a_0,__zpabi_f__a_1,__zpabi_f__b_0,__zpabi_f__b_1,__zpabi_f__c_0,__zpabi_f__c_1,__zpabi_f__d_0,__zpabi_f__d_1 locals=__local_f__0,__local_f__1,__local_f__2,__local_f__3,__local_f__4,__local_f__5,__local_f__6,__local_f__7,__local_f__a_1,__local_f__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def g params=__zpabi_g__w_0,__zpabi_g__w_1,__zpabi_g__x_0,__zpabi_g__x_1,__zpabi_g__y_0,__zpabi_g__y_1,__zpabi_g__z_0,__zpabi_g__z_1 locals=__local_g__w_1,__local_g__w_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call f -> g
; call main -> f
; item g section=.text local=false zero=false
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

g:
   SUBROUTINE

   STA   __local_g__w_0
   TXA
   BNE   .if_end@6
   LDA   __local_g__w_0
   CMP   #$02
   BNE   .if_end@6
   LDA   __zpabi_g__x_1
   BNE   .if_end@6
   LDA   __zpabi_g__x_0
   CMP   #$04
   BNE   .if_end@6
   LDA   __zpabi_g__y_1
   BNE   .if_end@6
   LDA   __zpabi_g__y_0
   CMP   #$06
   BNE   .if_end@6
   LDA   __zpabi_g__z_1
   BNE   .if_end@6
   LDA   __zpabi_g__z_0
   CMP   #$08
   BNE   .if_end@6
   LDX   #$00
   LDA   #$01
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS

f:
   SUBROUTINE

   STA   __local_f__a_0
   STX   __local_f__a_1
   ASL
   STA   __local_f__6
   TXA
   ROL
   STA   __local_f__7
   LDA   __zpabi_f__b_0
   ASL
   STA   __zpabi_g__x_0
   LDA   __zpabi_f__b_1
   ROL
   STA   __zpabi_g__x_1
   LDA   __zpabi_f__c_0
   ASL
   STA   __zpabi_g__y_0
   LDA   __zpabi_f__c_1
   ROL
   STA   __zpabi_g__y_1
   LDA   __zpabi_f__d_0
   ASL
   STA   __zpabi_g__z_0
   LDA   __zpabi_f__d_1
   ROL
   STA   __zpabi_g__z_1
   LDX   __local_f__7
   LDA   __local_f__6
   JSR   g
   STA   __zpabi_g__z_1
   TXA
   BNE   .and_false@7
   LDA   __zpabi_g__z_1
   CMP   #$01
   BNE   .and_false@7
   LDA   __local_f__a_1
   BNE   .and_false@7
   LDA   __local_f__a_0
   CMP   #$01
   BNE   .and_false@7
   LDA   __zpabi_f__b_1
   BNE   .and_false@7
   LDA   __zpabi_f__b_0
   CMP   #$02
   BNE   .and_false@7
   LDA   __zpabi_f__c_1
   BNE   .and_false@7
   LDA   __zpabi_f__c_0
   CMP   #$03
   BNE   .and_false@7
   LDA   __zpabi_f__d_1
   BNE   .and_false@7
   LDA   __zpabi_f__d_0
   CMP   #$04
   BNE   .and_false@7
   LDA   #$01
   LDX   #$00
   RTS
.and_false@7:
   LDA   #$00
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __zpabi_f__b_0
   LDA   #$03
   STA   __zpabi_f__c_0
   LDA   #$04
   STA   __zpabi_f__d_0
   LDA   #$00
   STA   __zpabi_f__b_1
   STA   __zpabi_f__c_1
   STA   __zpabi_f__d_1
   LDX   #$00
   LDA   #$01
   JSR   f
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
