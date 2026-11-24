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
__local_main__nan_is_nonzero_1	EQU	$88
__local_main__nan_is_nonzero_0	EQU	$89
__local_main__0	EQU	$8A
__local_main__1	EQU	$8B
__local_main__2	EQU	$8C
__local_main__3	EQU	$8D
__local_main__4	EQU	$8E
__local_main__5	EQU	$8F
__local_main__6	EQU	$90
__local_main__7	EQU	$91

; @zp-link-meta-begin
; def main params= locals=__local_main__nan_is_nonzero_1,__local_main__nan_is_nonzero_0,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; ext double_isnan params=__zpabi_double_isnan__d_0,__zpabi_double_isnan__d_1,__zpabi_double_isnan__d_2,__zpabi_double_isnan__d_3,__zpabi_double_isnan__d_4,__zpabi_double_isnan__d_5,__zpabi_double_isnan__d_6,__zpabi_double_isnan__d_7
; call main -> double_isnan
; item main section=.text local=false zero=false
; item main@run8 section=.text local=true zero=false
; item main@run7 section=.text local=true zero=false
; item main@run6 section=.text local=true zero=false
; item main@run5 section=.text local=true zero=false
; item main@run4 section=.text local=true zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item @1.zero section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   main@run5
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
   STA   __local_main__7
   STA   HARGS
   LDA   HARGS+17
   STA   __local_main__6
   STA   HARGS+1
   LDA   HARGS+18
   STA   __local_main__5
   STA   HARGS+2
   LDA   HARGS+19
   STA   __local_main__4
   STA   HARGS+3
   LDA   HARGS+20
   STA   __local_main__3
   STA   HARGS+4
   LDA   HARGS+21
   STA   __local_main__2
   STA   HARGS+5
   LDA   HARGS+22
   STA   __local_main__1
   STA   HARGS+6
   LDA   HARGS+23
   STA   __local_main__0
   JSR   main@run1
   STY   HARGS+13
   STY   HARGS+14
   JSR   main@run3
   BEQ   .lb_skip@5
   JMP   .main@jthread@3
.lb_skip@5:
   LDA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .lb_skip@4
   JMP   .main@jthread@3
.lb_skip@4:
   JSR   main@run5
   LDA   __local_main__7
   STA   HARGS+8
   LDA   __local_main__6
   STA   HARGS+9
   LDA   __local_main__5
   STA   HARGS+10
   LDA   __local_main__4
   STA   HARGS+11
   LDA   __local_main__3
   STA   HARGS+12
   LDA   __local_main__2
   STA   HARGS+13
   LDA   __local_main__1
   STA   HARGS+14
   LDA   __local_main__0
   JSR   main@run3
   BEQ   .lb_skip@3
   JMP   .main@jthread@3
.lb_skip@3:
   LDA   __local_main__7
   STA   HARGS
   LDA   __local_main__6
   STA   HARGS+1
   LDA   __local_main__5
   STA   HARGS+2
   LDA   __local_main__4
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS+4
   LDA   __local_main__2
   STA   HARGS+5
   LDA   __local_main__1
   STA   HARGS+6
   LDA   __local_main__0
   JSR   main@run1
   STY   HARGS+13
   STY   HARGS+14
   JSR   main@run4
   BNE   .main@jthread@3
   JSR   main@run5
   LDA   __local_main__7
   STA   HARGS+8
   LDA   __local_main__6
   STA   HARGS+9
   LDA   __local_main__5
   STA   HARGS+10
   LDA   __local_main__4
   STA   HARGS+11
   LDA   __local_main__3
   STA   HARGS+12
   LDA   __local_main__2
   STA   HARGS+13
   LDA   __local_main__1
   STA   HARGS+14
   LDA   __local_main__0
   JSR   main@run4
   BEQ   .if_end@8
.main@jthread@3:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@8:
   JSR   main@run6
   LDA   __local_main__7
   STA   HARGS+8
   LDA   __local_main__6
   STA   HARGS+9
   LDA   __local_main__5
   STA   HARGS+10
   LDA   __local_main__4
   STA   HARGS+11
   LDA   __local_main__3
   STA   HARGS+12
   LDA   __local_main__2
   STA   HARGS+13
   LDA   __local_main__1
   STA   HARGS+14
   LDA   __local_main__0
   JSR   main@run3
   BEQ   .lb_skip@2
   JMP   .main@jthread@7
.lb_skip@2:
   LDA   #$3F
   CMP   __local_main__0
   BNE   .jcmp_skip@0
   LDA   #$F0
   CMP   __local_main__1
   BNE   .jcmp_skip@0
   LDA   #$00
   CMP   __local_main__2
   BNE   .jcmp_skip@0
   CMP   __local_main__3
   BNE   .jcmp_skip@0
   CMP   __local_main__4
   BNE   .jcmp_skip@0
   CMP   __local_main__5
   BNE   .jcmp_skip@0
   CMP   __local_main__6
   BNE   .jcmp_skip@0
   CMP   __local_main__7
   BNE   .lb_skip@1
   JMP   .main@jthread@7
.lb_skip@1:
.jcmp_skip@0:
   LDA   __local_main__7
   STA   HARGS
   LDA   __local_main__6
   STA   HARGS+1
   LDA   __local_main__5
   STA   HARGS+2
   LDA   __local_main__4
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS+4
   LDA   __local_main__2
   STA   HARGS+5
   LDA   __local_main__1
   STA   HARGS+6
   LDA   __local_main__0
   JSR   main@run8
   JSR   main@run3
   BEQ   .lb_skip@0
   JMP   .main@jthread@7
.lb_skip@0:
   JSR   main@run6
   LDA   __local_main__7
   STA   HARGS+8
   LDA   __local_main__6
   STA   HARGS+9
   LDA   __local_main__5
   STA   HARGS+10
   LDA   __local_main__4
   STA   HARGS+11
   LDA   __local_main__3
   STA   HARGS+12
   LDA   __local_main__2
   STA   HARGS+13
   LDA   __local_main__1
   STA   HARGS+14
   LDA   __local_main__0
   JSR   main@run4
   BNE   .main@jthread@7
   LDA   __local_main__7
   STA   HARGS
   LDA   __local_main__6
   STA   HARGS+1
   LDA   __local_main__5
   STA   HARGS+2
   LDA   __local_main__4
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS+4
   LDA   __local_main__2
   STA   HARGS+5
   LDA   __local_main__1
   STA   HARGS+6
   LDA   __local_main__0
   JSR   main@run8
   JSR   main@run4
   BEQ   .if_end@17
.main@jthread@7:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@17:
   LDA   __local_main__0
   CMP   __local_main__0
   BNE   .vthread@0
   LDA   __local_main__1
   CMP   __local_main__1
   BNE   .if_end@18
   LDA   __local_main__2
   CMP   __local_main__2
   BNE   .if_end@18
   LDA   __local_main__3
   CMP   __local_main__3
   BNE   .if_end@18
   LDA   __local_main__4
   CMP   __local_main__4
   BNE   .if_end@18
   LDA   __local_main__5
   CMP   __local_main__5
   BNE   .if_end@18
   LDA   __local_main__6
   CMP   __local_main__6
   BNE   .if_end@18
   LDA   __local_main__7
   CMP   __local_main__7
   BNE   .if_end@18
   LDX   #$00
   LDA   #$03
   RTS
.if_end@18:
   LDA   __local_main__0
.vthread@0:
   CMP   __local_main__0
   BNE   .if_end@19
   LDA   __local_main__1
   CMP   __local_main__1
   BNE   .if_end@19
   LDA   __local_main__2
   CMP   __local_main__2
   BNE   .if_end@19
   LDA   __local_main__3
   CMP   __local_main__3
   BNE   .if_end@19
   LDA   __local_main__4
   CMP   __local_main__4
   BNE   .if_end@19
   LDA   __local_main__5
   CMP   __local_main__5
   BNE   .if_end@19
   LDA   __local_main__6
   CMP   __local_main__6
   BNE   .if_end@19
   LDA   __local_main__7
   CMP   __local_main__7
   BNE   .vthread@1
   LDX   #$00
   LDA   #$04
   RTS
.if_end@19:
   LDA   __local_main__7
.vthread@1:
   STA   __zpabi_double_isnan__d_0
   LDA   __local_main__6
   STA   __zpabi_double_isnan__d_1
   LDA   __local_main__5
   STA   __zpabi_double_isnan__d_2
   LDA   __local_main__4
   STA   __zpabi_double_isnan__d_3
   LDA   __local_main__3
   STA   __zpabi_double_isnan__d_4
   LDA   __local_main__2
   STA   __zpabi_double_isnan__d_5
   LDA   __local_main__1
   STA   __zpabi_double_isnan__d_6
   LDA   __local_main__0
   STA   __zpabi_double_isnan__d_7
   JSR   double_isnan
   STX   __local_main__nan_is_nonzero_1
   ORA   __local_main__nan_is_nonzero_1
   BNE   .if_end@20
   LDX   #$00
   LDA   #$05
   RTS
.if_end@20:
   JSR   main@run0
   STA   HARGS+5
   LDA   #$10
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   LDA   __local_main__7
   STA   HARGS+8
   LDA   __local_main__6
   STA   HARGS+9
   LDA   __local_main__5
   STA   HARGS+10
   LDA   __local_main__4
   STA   HARGS+11
   LDA   __local_main__3
   STA   HARGS+12
   LDA   __local_main__2
   STA   HARGS+13
   LDA   __local_main__1
   STA   HARGS+14
   LDA   __local_main__0
   STA   HARGS+15
   JSR   dmul
   JSR   main@run2
   STX   __local_main__nan_is_nonzero_1
   ORA   __local_main__nan_is_nonzero_1
   BNE   .if_end@21
   LDX   #$00
   LDA   #$06
   RTS
.if_end@21:
   JSR   main@run7
   STA   HARGS+4
   LDA   #$30
   STA   HARGS+5
   LDA   #$A1
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   LDA   __local_main__7
   STA   HARGS+8
   LDA   __local_main__6
   STA   HARGS+9
   LDA   __local_main__5
   STA   HARGS+10
   LDA   __local_main__4
   STA   HARGS+11
   LDA   __local_main__3
   STA   HARGS+12
   LDA   __local_main__2
   STA   HARGS+13
   LDA   __local_main__1
   STA   HARGS+14
   LDA   __local_main__0
   STA   HARGS+15
   JSR   ddiv
   JSR   main@run2
   STX   __local_main__nan_is_nonzero_1
   ORA   __local_main__nan_is_nonzero_1
   BNE   .if_end@22
   LDX   #$00
   LDA   #$07
   RTS
.if_end@22:
   LDA   __local_main__7
   STA   __zpabi_double_isnan__d_0
   LDA   __local_main__6
   STA   __zpabi_double_isnan__d_1
   LDA   __local_main__5
   STA   __zpabi_double_isnan__d_2
   LDA   __local_main__4
   STA   __zpabi_double_isnan__d_3
   LDA   __local_main__3
   STA   __zpabi_double_isnan__d_4
   LDA   __local_main__2
   STA   __zpabi_double_isnan__d_5
   LDA   __local_main__1
   STA   __zpabi_double_isnan__d_6
   LDA   __local_main__0
   EOR   #$80
   STA   __zpabi_double_isnan__d_7
   JSR   double_isnan
   STX   __local_main__nan_is_nonzero_1
   ORA   __local_main__nan_is_nonzero_1
   BNE   .if_end@23
   LDX   #$00
   LDA   #$08
   RTS
.if_end@23:
   LDA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .if_end@24
   LDX   #$00
   LDA   #$09
   RTS
.if_end@24:
   LDA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .if_end@25
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@25:
   LDA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .if_end@27
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@27:
   LDA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .if_end@28
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@28:
   LDA   #$FF
   STA   __local_main__nan_is_nonzero_0
   STA   __local_main__nan_is_nonzero_1
.loop@2_start:
   INC   __local_main__nan_is_nonzero_0
   BNE   .inc_done@0
   INC   __local_main__nan_is_nonzero_1
.inc_done@0:
   LDA   __local_main__nan_is_nonzero_0
   ORA   __local_main__nan_is_nonzero_1
   BNE   .if_end@30
   LDA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .loop@2_start
   LDA   __local_main__nan_is_nonzero_0
   ORA   __local_main__nan_is_nonzero_1
   BNE   .if_end@30
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@30:
   LDA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .if_end@33
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@33:
   LDX   #$00
   TXA
   RTS

main@run8:
   SUBROUTINE

   JSR   main@run1
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   RTS

main@run7:
   SUBROUTINE

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   RTS

main@run6:
   SUBROUTINE

   JSR   main@run0
   STA   HARGS+5
   LDA   #$F0
   STA   HARGS+6
   LDA   #$3F
   STA   HARGS+7
   RTS

main@run5:
   SUBROUTINE

   JSR   main@run0
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   RTS

main@run4:
   SUBROUTINE

   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   RTS

main@run3:
   SUBROUTINE

   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   RTS

main@run2:
   SUBROUTINE

   LDA   HARGS+16
   STA   __zpabi_double_isnan__d_0
   LDA   HARGS+17
   STA   __zpabi_double_isnan__d_1
   LDA   HARGS+18
   STA   __zpabi_double_isnan__d_2
   LDA   HARGS+19
   STA   __zpabi_double_isnan__d_3
   LDA   HARGS+20
   STA   __zpabi_double_isnan__d_4
   LDA   HARGS+21
   STA   __zpabi_double_isnan__d_5
   LDA   HARGS+22
   STA   __zpabi_double_isnan__d_6
   LDA   HARGS+23
   STA   __zpabi_double_isnan__d_7
   JMP   double_isnan

main@run1:
   SUBROUTINE

   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   RTS

main@run0:
   SUBROUTINE

   JSR   main@run7
   STY   HARGS+4
   RTS

@1.zero:
   DS.B  8
