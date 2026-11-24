SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_truncate__l_0	EQU	$80
__zpabi_truncate__l_1	EQU	$81
__zpabi_truncate__l_2	EQU	$82
__zpabi_truncate__l_3	EQU	$83
__zpabi_truncate__l_4	EQU	$84
__zpabi_truncate__l_5	EQU	$85
__zpabi_truncate__l_6	EQU	$86
__zpabi_truncate__l_7	EQU	$87
__zpabi_truncate__expected_0	EQU	$88
__zpabi_truncate__expected_1	EQU	$89
__local_main__0	EQU	$8A

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def truncate params=__zpabi_truncate__l_0,__zpabi_truncate__l_1,__zpabi_truncate__l_2,__zpabi_truncate__l_3,__zpabi_truncate__l_4,__zpabi_truncate__l_5,__zpabi_truncate__l_6,__zpabi_truncate__l_7,__zpabi_truncate__expected_0,__zpabi_truncate__expected_1 locals=__local_truncate__0 indirect=false in_cycle=false
; call main -> truncate
; item truncate section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

truncate:
   SUBROUTINE

   LDA   __zpabi_truncate__l_1
   CMP   __zpabi_truncate__expected_1
   BNE   .thread@0
   LDA   __zpabi_truncate__l_0
   CMP   __zpabi_truncate__expected_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$0A
   STA   __zpabi_truncate__l_0
   STA   __zpabi_truncate__expected_0
   LDA   #$00
   STA   __zpabi_truncate__l_1
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$F6
   JSR   main@run2
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$05
   STA   __zpabi_truncate__l_0
   STA   __zpabi_truncate__expected_0
   LDA   #$01
   STA   __zpabi_truncate__l_1
   LDA   #$00
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$05
   JSR   main@run2
   JSR   main@run1
   LDA   #$00
   STA   __zpabi_truncate__expected_1
   JSR   truncate
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   LDA   #$05
   RTS

main@run2:
   SUBROUTINE

   STA   __zpabi_truncate__l_0
   STA   __zpabi_truncate__expected_0
   LDA   #$FF
   STA   __zpabi_truncate__l_1
   RTS

main@run1:
   SUBROUTINE

   STA   __zpabi_truncate__l_2
   STA   __zpabi_truncate__l_3
   STA   __zpabi_truncate__l_4
   STA   __zpabi_truncate__l_5
   STA   __zpabi_truncate__l_6
   STA   __zpabi_truncate__l_7
   RTS

main@run0:
   SUBROUTINE

   JSR   main@run1
   STA   __zpabi_truncate__expected_1
   JMP   truncate
