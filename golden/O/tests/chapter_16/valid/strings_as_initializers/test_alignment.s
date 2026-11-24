SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__flat_auto	EQU	$82
__local_main__nested_auto	EQU	$98

; @zp-link-meta-begin
; def check_aligment params=__zpabi_check_aligment__c_0,__zpabi_check_aligment__c_1 locals=__local_check_aligment__c_0,__local_check_aligment__c_1 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__flat_auto,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__nested_auto,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39,__local_main__40,__local_main__41,__local_main__42,__local_main__43,__local_main__44,__local_main__45,__local_main__46,__local_main__47,__local_main__48,__local_main__49,__local_main__50,__local_main__51,__local_main__52,__local_main__53 indirect=false in_cycle=false
; call main -> check_aligment
; item check_aligment section=.text local=false zero=false
; item main section=.text local=false zero=false
; item flat_static section=.data local=true zero=false
; item nested_static section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check_aligment:
   SUBROUTINE

   AND   #$0F
   BEQ   .cmp_true@0
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDX   #>flat_static
   LDA   #<flat_static
   JSR   check_aligment
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #>nested_static
   LDA   #<nested_static
   JSR   check_aligment
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #>__local_main__flat_auto
   LDA   #<__local_main__flat_auto
   JSR   check_aligment
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #>__local_main__nested_auto
   LDA   #<__local_main__nested_auto
   JSR   check_aligment
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

flat_static:
   DC.B  $78, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

nested_static:
   DC.B  $61, $00
   DS.B  6
   DC.B  $62, $00
   DS.B  14
