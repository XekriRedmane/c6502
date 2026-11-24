SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_double_isnan__d_0	EQU	$80
__zpabi_double_isnan__d_1	EQU	$81
__zpabi_double_isnan__d_2	EQU	$82
__zpabi_double_isnan__d_3	EQU	$83
__zpabi_double_isnan__d_4	EQU	$84
__zpabi_double_isnan__d_5	EQU	$85
__zpabi_double_isnan__d_6	EQU	$86
__zpabi_double_isnan__d_7	EQU	$87
__local_main__0	EQU	$88
__local_main__2	EQU	$8A
__local_main__3	EQU	$8B
__local_main__4	EQU	$8C
__local_main__5	EQU	$8D
__local_main__6	EQU	$8E
__local_main__7	EQU	$8F
__local_main__8	EQU	$90
__local_main__9	EQU	$91
__local_main__10	EQU	$92
__local_main__11	EQU	$93
__local_main__12	EQU	$94
__local_main__13	EQU	$95
__local_main__14	EQU	$96
__local_main__15	EQU	$97
__local_main__16	EQU	$98
__local_main__17	EQU	$99

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17 indirect=false in_cycle=false
; ext double_isnan params=__zpabi_double_isnan__d_0,__zpabi_double_isnan__d_1,__zpabi_double_isnan__d_2,__zpabi_double_isnan__d_3,__zpabi_double_isnan__d_4,__zpabi_double_isnan__d_5,__zpabi_double_isnan__d_6,__zpabi_double_isnan__d_7
; call main -> double_isnan
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item @1.zero section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDA   @1.zero
   STA   HARGS+8
   LDA   @1.zero+1
   STA   HARGS+9
   LDA   @1.zero+2
   STA   HARGS+10
   LDA   @1.zero+3
   STA   HARGS+11
   LDA   @1.zero+4
   STA   HARGS+12
   LDA   @1.zero+5
   STA   HARGS+13
   LDA   @1.zero+6
   STA   HARGS+14
   LDA   @1.zero+7
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   STA   HARGS+4
   LDA   HARGS+21
   STA   HARGS+5
   LDA   HARGS+22
   STA   HARGS+6
   LDA   HARGS+23
   JSR   main@run2
   STA   __local_main__9
   LDA   HARGS+17
   STA   __local_main__8
   LDA   HARGS+18
   STA   __local_main__7
   LDA   HARGS+19
   STA   __local_main__6
   LDA   HARGS+20
   STA   __local_main__5
   LDA   HARGS+21
   STA   __local_main__4
   LDA   HARGS+22
   STA   __local_main__3
   LDA   HARGS+23
   STA   __local_main__2
   LDA   __local_main__9
   STA   __zpabi_double_isnan__d_0
   LDA   __local_main__8
   STA   __zpabi_double_isnan__d_1
   LDA   __local_main__7
   STA   __zpabi_double_isnan__d_2
   LDA   __local_main__6
   STA   __zpabi_double_isnan__d_3
   LDA   __local_main__5
   STA   __zpabi_double_isnan__d_4
   LDA   __local_main__4
   STA   __zpabi_double_isnan__d_5
   LDA   __local_main__3
   STA   __zpabi_double_isnan__d_6
   LDA   __local_main__2
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__9
   STA   HARGS
   LDA   __local_main__8
   STA   HARGS+1
   LDA   __local_main__7
   STA   HARGS+2
   LDA   __local_main__6
   STA   HARGS+3
   LDA   __local_main__5
   STA   HARGS+4
   LDA   __local_main__4
   STA   HARGS+5
   LDA   __local_main__3
   STA   HARGS+6
   LDA   __local_main__2
   JSR   main@run0
   JSR   dsub
   LDA   HARGS+16
   STA   __local_main__17
   LDA   HARGS+17
   STA   __local_main__16
   LDA   HARGS+18
   STA   __local_main__15
   LDA   HARGS+19
   STA   __local_main__14
   LDA   HARGS+20
   STA   __local_main__13
   LDA   HARGS+21
   STA   __local_main__12
   LDA   HARGS+22
   STA   __local_main__11
   LDA   HARGS+23
   STA   __local_main__10
   LDA   __local_main__17
   STA   __zpabi_double_isnan__d_0
   LDA   __local_main__16
   STA   __zpabi_double_isnan__d_1
   LDA   __local_main__15
   STA   __zpabi_double_isnan__d_2
   LDA   __local_main__14
   STA   __zpabi_double_isnan__d_3
   LDA   __local_main__13
   STA   __zpabi_double_isnan__d_4
   LDA   __local_main__12
   STA   __zpabi_double_isnan__d_5
   LDA   __local_main__11
   STA   __zpabi_double_isnan__d_6
   LDA   __local_main__10
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__17
   STA   HARGS
   LDA   __local_main__16
   STA   HARGS+1
   LDA   __local_main__15
   STA   HARGS+2
   LDA   __local_main__14
   STA   HARGS+3
   LDA   __local_main__13
   STA   HARGS+4
   LDA   __local_main__12
   STA   HARGS+5
   LDA   __local_main__11
   STA   HARGS+6
   LDA   __local_main__10
   JSR   main@run2
   STA   __local_main__9
   LDA   HARGS+17
   STA   __local_main__8
   LDA   HARGS+18
   STA   __local_main__7
   LDA   HARGS+19
   STA   __local_main__6
   LDA   HARGS+20
   STA   __local_main__5
   LDA   HARGS+21
   STA   __local_main__4
   LDA   HARGS+22
   STA   __local_main__3
   LDA   HARGS+23
   STA   __local_main__2
   LDA   __local_main__17
   STA   __zpabi_double_isnan__d_0
   LDA   __local_main__16
   STA   __zpabi_double_isnan__d_1
   LDA   __local_main__15
   STA   __zpabi_double_isnan__d_2
   LDA   __local_main__14
   STA   __zpabi_double_isnan__d_3
   LDA   __local_main__13
   STA   __zpabi_double_isnan__d_4
   LDA   __local_main__12
   STA   __zpabi_double_isnan__d_5
   LDA   __local_main__11
   STA   __zpabi_double_isnan__d_6
   LDA   __local_main__10
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__9
   STA   __zpabi_double_isnan__d_0
   LDA   __local_main__8
   STA   __zpabi_double_isnan__d_1
   LDA   __local_main__7
   STA   __zpabi_double_isnan__d_2
   LDA   __local_main__6
   STA   __zpabi_double_isnan__d_3
   LDA   __local_main__5
   STA   __zpabi_double_isnan__d_4
   LDA   __local_main__4
   STA   __zpabi_double_isnan__d_5
   LDA   __local_main__3
   STA   __zpabi_double_isnan__d_6
   LDA   __local_main__2
   JSR   main@run1
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

main@run2:
   SUBROUTINE

   JSR   main@run0
   JSR   dadd
   LDA   HARGS+16
   RTS

main@run1:
   SUBROUTINE

   STA   __zpabi_double_isnan__d_7
   JMP   double_isnan

main@run0:
   SUBROUTINE

   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   RTS

@1.zero:
   DS.B  8
