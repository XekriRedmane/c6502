SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_switch_on_uint__ui_0	EQU	$80
__zpabi_switch_on_uint__ui_1	EQU	$81
__zpabi_switch_on_uint__ui_2	EQU	$82
__zpabi_switch_on_uint__ui_3	EQU	$83
__local_main__0	EQU	$84
__local_main__1	EQU	$85

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def switch_on_uint params=__zpabi_switch_on_uint__ui_0,__zpabi_switch_on_uint__ui_1,__zpabi_switch_on_uint__ui_2,__zpabi_switch_on_uint__ui_3 locals= indirect=false in_cycle=false
; call main -> switch_on_uint
; item switch_on_uint section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

switch_on_uint:
   SUBROUTINE

   LDA   __zpabi_switch_on_uint__ui_3
   BNE   .default@4
   LDA   __zpabi_switch_on_uint__ui_2
   BNE   .vthread@0
   LDA   __zpabi_switch_on_uint__ui_1
   BNE   .vthread@1
   LDA   __zpabi_switch_on_uint__ui_0
   CMP   #$05
   BEQ   .case@1
   LDA   __zpabi_switch_on_uint__ui_3
   BNE   .default@4
   LDA   __zpabi_switch_on_uint__ui_2
   BNE   .vthread@0
   LDA   __zpabi_switch_on_uint__ui_1
.vthread@1:
   CMP   #$FF
   BNE   .default@4
   LDA   __zpabi_switch_on_uint__ui_0
   CMP   #$F6
   BEQ   .case@2
   LDA   __zpabi_switch_on_uint__ui_3
   BNE   .default@4
   LDA   __zpabi_switch_on_uint__ui_2
.vthread@0:
   CMP   #$02
   BNE   .default@4
   LDA   __zpabi_switch_on_uint__ui_1
   BNE   .default@4
   LDA   __zpabi_switch_on_uint__ui_0
   CMP   #$0A
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

   LDA   #$05
   STA   __zpabi_switch_on_uint__ui_0
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$F6
   STA   __zpabi_switch_on_uint__ui_0
   LDA   #$FF
   STA   __zpabi_switch_on_uint__ui_1
   LDA   #$00
   STA   __zpabi_switch_on_uint__ui_2
   STA   __zpabi_switch_on_uint__ui_3
   JSR   switch_on_uint
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
   LDA   #$0A
   STA   __zpabi_switch_on_uint__ui_0
   JSR   main@run0
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
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_switch_on_uint__ui_1
   STA   __zpabi_switch_on_uint__ui_2
   STA   __zpabi_switch_on_uint__ui_3
   JMP   switch_on_uint
