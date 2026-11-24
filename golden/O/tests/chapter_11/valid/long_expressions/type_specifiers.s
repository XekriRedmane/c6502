SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_my_function__x_0	EQU	$80
__zpabi_my_function__x_1	EQU	$81
__zpabi_my_function__x_2	EQU	$82
__zpabi_my_function__x_3	EQU	$83
__zpabi_my_function__x_4	EQU	$84
__zpabi_my_function__x_5	EQU	$85
__zpabi_my_function__x_6	EQU	$86
__zpabi_my_function__x_7	EQU	$87
__zpabi_my_function__y_0	EQU	$88
__zpabi_my_function__y_1	EQU	$89
__zpabi_my_function__y_2	EQU	$8A
__zpabi_my_function__y_3	EQU	$8B
__zpabi_my_function__y_4	EQU	$8C
__zpabi_my_function__y_5	EQU	$8D
__zpabi_my_function__y_6	EQU	$8E
__zpabi_my_function__y_7	EQU	$8F
__zpabi_my_function__z_0	EQU	$90
__zpabi_my_function__z_1	EQU	$91
__zpabi_my_function__z_2	EQU	$92
__zpabi_my_function__z_3	EQU	$93
__zpabi_my_function__z_4	EQU	$94
__zpabi_my_function__z_5	EQU	$95
__zpabi_my_function__z_6	EQU	$96
__zpabi_my_function__z_7	EQU	$97
__local_main__i_7	EQU	$98
__local_main__i_6	EQU	$99
__local_main__i_5	EQU	$9A
__local_main__i_4	EQU	$9B
__local_main__i_3	EQU	$9C
__local_main__i_2	EQU	$9D
__local_main__i_1	EQU	$9E
__local_main__i_0	EQU	$9F
__local_main__sum_1	EQU	$A0
__local_main__sum_0	EQU	$A1
__local_my_function__0	EQU	$A2
__local_my_function__1	EQU	$A3

; @zp-link-meta-begin
; def main params= locals=__local_main__i_7,__local_main__i_6,__local_main__i_5,__local_main__i_4,__local_main__i_3,__local_main__i_2,__local_main__i_1,__local_main__i_0,__local_main__sum_1,__local_main__sum_0 indirect=false in_cycle=false
; def my_function params=__zpabi_my_function__x_0,__zpabi_my_function__x_1,__zpabi_my_function__x_2,__zpabi_my_function__x_3,__zpabi_my_function__x_4,__zpabi_my_function__x_5,__zpabi_my_function__x_6,__zpabi_my_function__x_7,__zpabi_my_function__y_0,__zpabi_my_function__y_1,__zpabi_my_function__y_2,__zpabi_my_function__y_3,__zpabi_my_function__y_4,__zpabi_my_function__y_5,__zpabi_my_function__y_6,__zpabi_my_function__y_7,__zpabi_my_function__z_0,__zpabi_my_function__z_1,__zpabi_my_function__z_2,__zpabi_my_function__z_3,__zpabi_my_function__z_4,__zpabi_my_function__z_5,__zpabi_my_function__z_6,__zpabi_my_function__z_7 locals=__local_my_function__0,__local_my_function__1 indirect=false in_cycle=false
; call main -> my_function
; item my_function section=.text local=false zero=false
; item main section=.text local=false zero=false
; item a section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

my_function:
   SUBROUTINE

   LDA   __zpabi_my_function__x_0
   CLC
   ADC   __zpabi_my_function__y_0
   STA   __local_my_function__0
   LDA   __zpabi_my_function__x_1
   ADC   __zpabi_my_function__y_1
   STA   __local_my_function__1
   LDA   __local_my_function__0
   CLC
   ADC   __zpabi_my_function__z_0
   STA   __local_my_function__0
   LDA   __local_my_function__1
   ADC   __zpabi_my_function__z_1
   TAX
   LDA   __local_my_function__0
   RTS

main:
   SUBROUTINE

   LDA   #$04
   STA   a
   LDA   #$40
   STA   __local_main__i_3
   LDA   #$00
   STA   a+1
   STA   a+2
   STA   a+3
   STA   a+4
   STA   a+5
   STA   a+6
   STA   a+7
   STA   __local_main__sum_0
   STA   __local_main__sum_1
   STA   __local_main__i_0
   STA   __local_main__i_1
   STA   __local_main__i_2
   STA   __local_main__i_4
   STA   __local_main__i_5
   STA   __local_main__i_6
   STA   __local_main__i_7
.loop@0_start:
   BPL   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   ORA   __local_main__i_0
   ORA   __local_main__i_1
   ORA   __local_main__i_2
   ORA   __local_main__i_3
   ORA   __local_main__i_4
   ORA   __local_main__i_5
   ORA   __local_main__i_6
   BEQ   .loop@0_break
   INC   __local_main__sum_0
   BNE   .inc_done@0
   INC   __local_main__sum_1
.inc_done@0:
   LDA   __local_main__i_0
   STA   HARGS
   LDA   __local_main__i_1
   STA   HARGS+1
   LDA   __local_main__i_2
   STA   HARGS+2
   LDA   __local_main__i_3
   STA   HARGS+3
   LDA   __local_main__i_4
   STA   HARGS+4
   LDA   __local_main__i_5
   STA   HARGS+5
   LDA   __local_main__i_6
   STA   HARGS+6
   LDA   __local_main__i_7
   STA   HARGS+7
   LDA   #$02
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+16
   STA   __local_main__i_0
   LDA   HARGS+17
   STA   __local_main__i_1
   LDA   HARGS+18
   STA   __local_main__i_2
   LDA   HARGS+19
   STA   __local_main__i_3
   LDA   HARGS+20
   STA   __local_main__i_4
   LDA   HARGS+21
   STA   __local_main__i_5
   LDA   HARGS+22
   STA   __local_main__i_6
   LDA   HARGS+23
   STA   __local_main__i_7
   JMP   .loop@0_start
.loop@0_break:
   LDA   a+7
   BNE   .jcmp_skip@0
   LDA   a+6
   BNE   .jcmp_skip@0
   LDA   a+5
   BNE   .jcmp_skip@0
   LDA   a+4
   BNE   .jcmp_skip@0
   LDA   a+3
   BNE   .jcmp_skip@0
   LDA   a+2
   BNE   .jcmp_skip@0
   LDA   a+1
   BNE   .jcmp_skip@0
   LDA   a
   CMP   #$04
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$01
   STA   __zpabi_my_function__x_0
   LDA   #$02
   STA   __zpabi_my_function__y_0
   LDA   #$03
   STA   __zpabi_my_function__z_0
   LDA   #$00
   STA   __zpabi_my_function__x_1
   STA   __zpabi_my_function__x_2
   STA   __zpabi_my_function__x_3
   STA   __zpabi_my_function__x_4
   STA   __zpabi_my_function__x_5
   STA   __zpabi_my_function__x_6
   STA   __zpabi_my_function__x_7
   STA   __zpabi_my_function__y_1
   STA   __zpabi_my_function__y_2
   STA   __zpabi_my_function__y_3
   STA   __zpabi_my_function__y_4
   STA   __zpabi_my_function__y_5
   STA   __zpabi_my_function__y_6
   STA   __zpabi_my_function__y_7
   STA   __zpabi_my_function__z_1
   STA   __zpabi_my_function__z_2
   STA   __zpabi_my_function__z_3
   STA   __zpabi_my_function__z_4
   STA   __zpabi_my_function__z_5
   STA   __zpabi_my_function__z_6
   STA   __zpabi_my_function__z_7
   JSR   my_function
   STA   __local_main__i_6
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__i_6
   CMP   #$06
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__sum_1
   BNE   .jcmp_skip@2
   LDA   __local_main__sum_0
   CMP   #$1F
   BEQ   .if_end@4
.jcmp_skip@2:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

a:
   DS.B  8
