SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_switch_on_long__l_0	EQU	$80
__zpabi_switch_on_long__l_1	EQU	$81
__zpabi_switch_on_long__l_2	EQU	$82
__zpabi_switch_on_long__l_3	EQU	$83
__zpabi_switch_on_long__l_4	EQU	$84
__zpabi_switch_on_long__l_5	EQU	$85
__zpabi_switch_on_long__l_6	EQU	$86
__zpabi_switch_on_long__l_7	EQU	$87
__local_main__1	EQU	$89

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def switch_on_long params=__zpabi_switch_on_long__l_0,__zpabi_switch_on_long__l_1,__zpabi_switch_on_long__l_2,__zpabi_switch_on_long__l_3,__zpabi_switch_on_long__l_4,__zpabi_switch_on_long__l_5,__zpabi_switch_on_long__l_6,__zpabi_switch_on_long__l_7 locals= indirect=false in_cycle=false
; call main -> switch_on_long
; item switch_on_long section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

switch_on_long:
   SUBROUTINE

   LDA   __zpabi_switch_on_long__l_0
   ORA   __zpabi_switch_on_long__l_1
   ORA   __zpabi_switch_on_long__l_2
   ORA   __zpabi_switch_on_long__l_3
   ORA   __zpabi_switch_on_long__l_4
   ORA   __zpabi_switch_on_long__l_5
   ORA   __zpabi_switch_on_long__l_6
   ORA   __zpabi_switch_on_long__l_7
   BEQ   .case@1
   LDA   __zpabi_switch_on_long__l_7
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_6
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_5
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_4
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_3
   BNE   .vthread@0
   LDA   __zpabi_switch_on_long__l_2
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_1
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_0
   CMP   #$64
   BEQ   .case@2
   LDA   __zpabi_switch_on_long__l_7
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_6
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_5
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_4
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_3
.vthread@0:
   CMP   #$40
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_2
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_1
   BNE   .default@4
   LDA   __zpabi_switch_on_long__l_0
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
   LDX   #$FF
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$40
   STA   __zpabi_switch_on_long__l_3
   LDA   #$00
   STA   __zpabi_switch_on_long__l_0
   STA   __zpabi_switch_on_long__l_1
   STA   __zpabi_switch_on_long__l_2
   JSR   main@run0
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@0
.jcmp_skip@2:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$64
   STA   __zpabi_switch_on_long__l_0
   LDA   #$00
   STA   __zpabi_switch_on_long__l_1
   STA   __zpabi_switch_on_long__l_2
   STA   __zpabi_switch_on_long__l_3
   JSR   main@run0
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
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   STA   __zpabi_switch_on_long__l_4
   STA   __zpabi_switch_on_long__l_5
   STA   __zpabi_switch_on_long__l_6
   STA   __zpabi_switch_on_long__l_7
   JMP   switch_on_long
