SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__flag2_0	EQU	$80
__zpabi_target__flag2_1	EQU	$81
__zpabi_target__y_0	EQU	$82
__zpabi_target__y_1	EQU	$83
__zpabi_target__y_2	EQU	$84
__zpabi_target__y_3	EQU	$85
__zpabi_target__y_4	EQU	$86
__zpabi_target__y_5	EQU	$87
__zpabi_target__y_6	EQU	$88
__zpabi_target__y_7	EQU	$89
__zpabi_target__y_8	EQU	$8A
__zpabi_target__y_9	EQU	$8B
__local_target__flag_1	EQU	$94
__local_target__x_7	EQU	$96
__local_target__x_6	EQU	$97
__local_target__x_5	EQU	$98
__local_target__x_4	EQU	$99
__local_target__x_3	EQU	$9A
__local_target__x_2	EQU	$9B
__local_target__x_1	EQU	$9C
__local_target__x_0	EQU	$9D

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1,__zpabi_target__flag2_0,__zpabi_target__flag2_1,__zpabi_target__y_0,__zpabi_target__y_1,__zpabi_target__y_2,__zpabi_target__y_3,__zpabi_target__y_4,__zpabi_target__y_5,__zpabi_target__y_6,__zpabi_target__y_7,__zpabi_target__y_8,__zpabi_target__y_9 locals=__local_target__flag_1,__local_target__flag_0,__local_target__x_7,__local_target__x_6,__local_target__x_5,__local_target__x_4,__local_target__x_3,__local_target__x_2,__local_target__x_1,__local_target__x_0,__local_target__x_9,__local_target__x_8,__local_target__0,__local_target__1,__local_target__2,__local_target__3,__local_target__4,__local_target__5,__local_target__6,__local_target__7 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,, changes=__zpabi_target__y_0,__zpabi_target__y_1,__zpabi_target__y_2,__zpabi_target__y_3,__zpabi_target__y_4,__zpabi_target__y_5,__zpabi_target__y_6,__zpabi_target__y_7,__zpabi_target__y_8,__zpabi_target__y_9
; call main -> target
; item target section=.text local=false zero=false
; item target@run1 section=.text local=true zero=false
; item target@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   __zpabi_target__y_0
   STA   HARGS+8
   STA   HARGS
   LDA   __zpabi_target__y_0+1
   STA   HARGS+9
   LDA   __zpabi_target__y_0+2
   STA   HARGS+10
   LDA   __zpabi_target__y_0+3
   STA   HARGS+11
   LDA   __zpabi_target__y_0+4
   STA   HARGS+12
   LDA   __zpabi_target__y_0+5
   STA   HARGS+13
   LDA   __zpabi_target__y_0+6
   STA   HARGS+14
   LDA   __zpabi_target__y_0+7
   STA   HARGS+15
   LDA   __zpabi_target__y_1
   STA   HARGS+1
   LDA   __zpabi_target__y_2
   STA   HARGS+2
   LDA   __zpabi_target__y_3
   STA   HARGS+3
   LDA   __zpabi_target__y_4
   STA   HARGS+4
   LDA   __zpabi_target__y_5
   STA   HARGS+5
   LDA   __zpabi_target__y_6
   STA   HARGS+6
   LDA   __zpabi_target__y_7
   STA   HARGS+7
   JSR   dadd
   LDA   HARGS+16
   STA   __local_target__x_0
   LDA   HARGS+17
   STA   __local_target__x_1
   LDA   HARGS+18
   STA   __local_target__x_2
   LDA   HARGS+19
   STA   __local_target__x_3
   LDA   HARGS+20
   STA   __local_target__x_4
   LDA   HARGS+21
   STA   __local_target__x_5
   LDA   HARGS+22
   STA   __local_target__x_6
   LDA   HARGS+23
   STA   __local_target__x_7
   LDA   __zpabi_target__y_8
   STA   HARGS
   LDA   __zpabi_target__y_9
   JSR   target@run1
   STX   __local_target__flag_1
   JSR   target@run0
   LDX   __local_target__x_0
   STX   HARGS
   LDX   __local_target__x_1
   STX   HARGS+1
   LDX   __local_target__x_2
   STX   HARGS+2
   LDX   __local_target__x_3
   STX   HARGS+3
   LDX   __local_target__x_4
   STX   HARGS+4
   LDX   __local_target__x_5
   STX   HARGS+5
   LDX   __local_target__x_6
   STX   HARGS+6
   LDX   __local_target__x_7
   STX   HARGS+7
   STA   HARGS+8
   LDA   __local_target__flag_1
   STA   HARGS+9
   JSR   dadd
   LDA   HARGS+16
   STA   __local_target__x_0
   LDA   HARGS+17
   STA   __local_target__x_1
   LDA   HARGS+18
   STA   __local_target__x_2
   LDA   HARGS+19
   STA   __local_target__x_3
   LDA   HARGS+20
   STA   __local_target__x_4
   LDA   HARGS+21
   STA   __local_target__x_5
   LDA   HARGS+22
   STA   __local_target__x_6
   LDA   HARGS+23
   STA   __local_target__x_7
   LDA   __zpabi_target__y_0+8
   STA   HARGS
   LDA   __zpabi_target__y_0+9
   JSR   target@run1
   STX   __local_target__flag_1
   JSR   target@run0
   LDX   __local_target__x_0
   STX   HARGS
   LDX   __local_target__x_1
   STX   HARGS+1
   LDX   __local_target__x_2
   STX   HARGS+2
   LDX   __local_target__x_3
   STX   HARGS+3
   LDX   __local_target__x_4
   STX   HARGS+4
   LDX   __local_target__x_5
   STX   HARGS+5
   LDX   __local_target__x_6
   STX   HARGS+6
   LDX   __local_target__x_7
   STX   HARGS+7
   STA   HARGS+8
   LDA   __local_target__flag_1
   STA   HARGS+9
   JMP   dadd

target@run1:
   SUBROUTINE

   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDX   HARGS+3
   RTS

target@run0:
   SUBROUTINE

   LDX   HARGS+4
   STX   HARGS+10
   LDX   HARGS+5
   STX   HARGS+11
   LDX   HARGS+6
   STX   HARGS+12
   LDX   HARGS+7
   STX   HARGS+13
   LDX   HARGS+8
   STX   HARGS+14
   LDX   HARGS+9
   STX   HARGS+15
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_target__y_0
   LDY   #$00
   STY   __zpabi_target__y_1
   STY   __zpabi_target__y_2
   STY   __zpabi_target__y_3
   STY   __zpabi_target__y_4
   STA   __zpabi_target__y_5
   LDA   #$39
   STA   __zpabi_target__y_6
   LDA   #$40
   STA   __zpabi_target__y_7
   LDA   #$05
   STA   __zpabi_target__y_8
   STY   __zpabi_target__y_9
   LDA   #$01
   STA   __zpabi_target__flag2_0
   LDA   #$00
   STA   __zpabi_target__flag2_1
   LDX   #$00
   JSR   target
   LDA   HARGS+23
   CMP   #$40
   BNE   .thread@0
   LDA   HARGS+22
   CMP   #$4E
   BNE   .thread@0
   LDA   HARGS+21
   BNE   .thread@0
   LDA   HARGS+20
   BNE   .thread@0
   LDA   HARGS+19
   BNE   .thread@0
   LDA   HARGS+18
   BNE   .thread@0
   LDA   HARGS+17
   BNE   .thread@0
   LDA   HARGS+16
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS
