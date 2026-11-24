SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_addition__1	EQU	$83
__local_complement__1	EQU	$83
__local_subtraction__1	EQU	$83
__local_addition__2	EQU	$84
__local_complement__2	EQU	$84
__local_subtraction__2	EQU	$84
__local_addition__3	EQU	$85
__local_complement__3	EQU	$85
__local_subtraction__3	EQU	$85
__local_subtraction__4	EQU	$86
__local_subtraction__5	EQU	$87
__local_subtraction__6	EQU	$88
__local_subtraction__7	EQU	$89

; @zp-link-meta-begin
; def addition params= locals=__local_addition__0,__local_addition__1,__local_addition__2,__local_addition__3 indirect=false in_cycle=false
; def complement params= locals=__local_complement__0,__local_complement__1,__local_complement__2,__local_complement__3 indirect=false in_cycle=false
; def division params= locals=__local_division__0,__local_division__1,__local_division__2,__local_division__3 indirect=false in_cycle=false
; def division_by_literal params= locals=__local_division_by_literal__0,__local_division_by_literal__1,__local_division_by_literal__2,__local_division_by_literal__3,__local_division_by_literal__4,__local_division_by_literal__5,__local_division_by_literal__6,__local_division_by_literal__7 indirect=false in_cycle=false
; def division_large_dividend params= locals=__local_division_large_dividend__0,__local_division_large_dividend__1,__local_division_large_dividend__2,__local_division_large_dividend__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def multiplication params= locals=__local_multiplication__0,__local_multiplication__1,__local_multiplication__2,__local_multiplication__3 indirect=false in_cycle=false
; def remaind params= locals=__local_remaind__0,__local_remaind__1,__local_remaind__2,__local_remaind__3,__local_remaind__4,__local_remaind__5,__local_remaind__6,__local_remaind__7 indirect=false in_cycle=false
; def subtraction params= locals=__local_subtraction__0,__local_subtraction__1,__local_subtraction__2,__local_subtraction__3,__local_subtraction__4,__local_subtraction__5,__local_subtraction__6,__local_subtraction__7 indirect=false in_cycle=false
; call main -> addition
; call main -> complement
; call main -> division
; call main -> division_by_literal
; call main -> division_large_dividend
; call main -> multiplication
; call main -> remaind
; call main -> subtraction
; item addition section=.text local=false zero=false
; item subtraction section=.text local=false zero=false
; item multiplication section=.text local=false zero=false
; item multiplication@run0 section=.text local=true zero=false
; item division section=.text local=false zero=false
; item division@run5 section=.text local=true zero=false
; item division_large_dividend section=.text local=false zero=false
; item division_by_literal section=.text local=false zero=false
; item remaind section=.text local=false zero=false
; item complement section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run4 section=.text local=true zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item ui_a section=.bss local=false zero=true
; item ui_b section=.bss local=false zero=true
; item ul_a section=.bss local=false zero=true
; item ul_b section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=32
; @zp-link-meta-end

addition:
   SUBROUTINE

   LDA   ui_a
   CLC
   ADC   #$05
   STA   __local_addition__3
   LDA   ui_a+1
   ADC   #$80
   STA   __local_addition__2
   LDA   ui_a+2
   ADC   #$00
   STA   __local_addition__1
   LDA   ui_a+3
   ADC   #$00
   BNE   .thread@0
   LDA   __local_addition__1
   BNE   .thread@0
   LDA   __local_addition__2
   CMP   #$80
   BNE   .thread@0
   LDA   __local_addition__3
   CMP   #$0F
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

subtraction:
   SUBROUTINE

   LDA   ul_a
   SEC
   SBC   ul_b
   STA   __local_subtraction__7
   LDA   ul_a+1
   SBC   ul_b+1
   STA   __local_subtraction__6
   LDA   ul_a+2
   SBC   ul_b+2
   STA   __local_subtraction__5
   LDA   ul_a+3
   SBC   ul_b+3
   STA   __local_subtraction__4
   LDA   ul_a+4
   SBC   ul_b+4
   STA   __local_subtraction__3
   LDA   ul_a+5
   SBC   ul_b+5
   STA   __local_subtraction__2
   LDA   ul_a+6
   SBC   ul_b+6
   STA   __local_subtraction__1
   LDA   ul_a+7
   SBC   ul_b+7
   BNE   .thread@1
   LDA   __local_subtraction__1
   BNE   .thread@1
   LDA   __local_subtraction__2
   BNE   .thread@1
   LDA   __local_subtraction__3
   BNE   .thread@1
   LDA   __local_subtraction__4
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__5
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__6
   CMP   #$7C
   BNE   .thread@1
   LDA   __local_subtraction__7
   CMP   #$18
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

multiplication:
   SUBROUTINE

   JSR   multiplication@run0
   JSR   mul32
   LDA   HARGS+11
   BNE   .thread@2
   LDA   HARGS+10
   BNE   .thread@2
   LDA   HARGS+9
   CMP   #$C0
   BNE   .thread@2
   LDA   HARGS+8
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

multiplication@run0:
   SUBROUTINE

   LDA   ui_a
   STA   HARGS
   LDA   ui_a+1
   STA   HARGS+1
   LDA   ui_a+2
   STA   HARGS+2
   LDA   ui_a+3
   STA   HARGS+3
   LDA   ui_b
   STA   HARGS+4
   LDA   ui_b+1
   STA   HARGS+5
   LDA   ui_b+2
   STA   HARGS+6
   LDA   ui_b+3
   STA   HARGS+7
   RTS

division:
   SUBROUTINE

   JSR   division@run5
   BNE   .thread@3
   LDA   HARGS+10
   BNE   .thread@3
   LDA   HARGS+9
   BNE   .thread@3
   LDA   HARGS+8
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
   RTS

division@run5:
   SUBROUTINE

   JSR   multiplication@run0
   JSR   udivmod32
   LDA   HARGS+11
   RTS

division_large_dividend:
   SUBROUTINE

   JSR   division@run5
   BNE   .thread@4
   LDA   HARGS+10
   BNE   .thread@4
   LDA   HARGS+9
   BNE   .thread@4
   LDA   HARGS+8
   CMP   #$02
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDX   #$00
   RTS

division_by_literal:
   SUBROUTINE

   LDA   ul_a
   STA   HARGS
   LDA   ul_a+1
   STA   HARGS+1
   LDA   ul_a+2
   STA   HARGS+2
   LDA   ul_a+3
   STA   HARGS+3
   LDA   ul_a+4
   STA   HARGS+4
   LDA   ul_a+5
   STA   HARGS+5
   LDA   ul_a+6
   STA   HARGS+6
   LDA   ul_a+7
   STA   HARGS+7
   LDA   #$05
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
   JSR   udivmod64
   LDA   HARGS+23
   BNE   .thread@5
   LDA   HARGS+22
   BNE   .thread@5
   LDA   HARGS+21
   BNE   .thread@5
   LDA   HARGS+20
   BNE   .thread@5
   LDA   HARGS+19
   BNE   .thread@5
   LDA   HARGS+18
   CMP   #$33
   BNE   .thread@5
   LDA   HARGS+17
   CMP   #$33
   BNE   .thread@5
   LDA   HARGS+16
   CMP   #$33
   BEQ   .cmp_true@15
.thread@5:
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDX   #$00
   RTS

remaind:
   SUBROUTINE

   LDA   ul_b
   STA   HARGS
   LDA   ul_b+1
   STA   HARGS+1
   LDA   ul_b+2
   STA   HARGS+2
   LDA   ul_b+3
   STA   HARGS+3
   LDA   ul_b+4
   STA   HARGS+4
   LDA   ul_b+5
   STA   HARGS+5
   LDA   ul_b+6
   STA   HARGS+6
   LDA   ul_b+7
   STA   HARGS+7
   LDA   ul_a
   STA   HARGS+8
   LDA   ul_a+1
   STA   HARGS+9
   LDA   ul_a+2
   STA   HARGS+10
   LDA   ul_a+3
   STA   HARGS+11
   LDA   ul_a+4
   STA   HARGS+12
   LDA   ul_a+5
   STA   HARGS+13
   LDA   ul_a+6
   STA   HARGS+14
   LDA   ul_a+7
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+31
   BNE   .thread@6
   LDA   HARGS+30
   BNE   .thread@6
   LDA   HARGS+29
   BNE   .thread@6
   LDA   HARGS+28
   BNE   .thread@6
   LDA   HARGS+27
   BNE   .thread@6
   LDA   HARGS+26
   BNE   .thread@6
   LDA   HARGS+25
   BNE   .thread@6
   LDA   HARGS+24
   CMP   #$05
   BEQ   .cmp_true@18
.thread@6:
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDX   #$00
   RTS

complement:
   SUBROUTINE

   LDA   ui_a
   EOR   #$FF
   STA   __local_complement__3
   LDA   ui_a+1
   EOR   #$FF
   STA   __local_complement__2
   LDA   ui_a+2
   EOR   #$FF
   STA   __local_complement__1
   LDA   ui_a+3
   EOR   #$FF
   BNE   .thread@7
   LDA   __local_complement__1
   BNE   .thread@7
   LDA   __local_complement__2
   BNE   .thread@7
   LDA   __local_complement__3
   BEQ   .cmp_true@21
.thread@7:
   LDA   #$00
   BEQ   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$0A
   STA   ui_a
   LDA   #$00
   STA   ui_a+1
   JSR   main@run2
   JSR   addition
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$80
   STA   ul_a+1
   LDA   #$FF
   STA   ul_a+2
   STA   ul_a+3
   LDA   #$E8
   STA   ul_b
   LDA   #$03
   STA   ul_b+1
   LDA   #$00
   STA   ul_a
   JSR   main@run1
   STA   ul_b+2
   STA   ul_b+3
   JSR   main@run3
   JSR   subtraction
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$40
   STA   ui_a+1
   LDA   #$03
   STA   ui_b
   LDA   #$00
   STA   ui_a
   JSR   main@run2
   STA   ui_b+1
   JSR   main@run4
   JSR   multiplication
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$64
   STA   ui_a
   LDA   #$FE
   STA   ui_b
   LDA   #$FF
   STA   ui_b+1
   LDA   #$00
   STA   ui_a+1
   JSR   main@run2
   JSR   main@run4
   JSR   division
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$FE
   STA   ui_a
   LDA   #$FF
   STA   ui_a+1
   STA   ui_b
   LDA   #$7F
   STA   ui_b+1
   LDA   #$00
   JSR   main@run2
   JSR   main@run4
   JSR   division_large_dividend
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$FF
   STA   ul_a
   STA   ul_a+1
   STA   ul_a+2
   LDA   #$00
   STA   ul_a+3
   JSR   main@run1
   JSR   division_by_literal
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #$64
   STA   ul_a
   LDA   #$A5
   STA   ul_b
   LDA   #$FF
   STA   ul_b+1
   STA   ul_b+2
   STA   ul_b+3
   LDA   #$00
   STA   ul_a+1
   STA   ul_a+2
   STA   ul_a+3
   JSR   main@run1
   JSR   main@run3
   JSR   remaind
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   #$FF
   STA   ui_a
   STA   ui_a+1
   LDA   #$00
   STA   ui_a+2
   STA   ui_a+3
   JSR   complement
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

main@run4:
   SUBROUTINE

   STA   ui_b+2
   STA   ui_b+3
   RTS

main@run3:
   SUBROUTINE

   STA   ul_b+4
   STA   ul_b+5
   STA   ul_b+6
   STA   ul_b+7
   RTS

main@run2:
   SUBROUTINE

   STA   ui_a+2
   STA   ui_a+3
   RTS

main@run1:
   SUBROUTINE

   STA   ul_a+4
   STA   ul_a+5
   STA   ul_a+6
   STA   ul_a+7
   RTS

ui_a:
   DS.B  4

ui_b:
   DS.B  4

ul_a:
   DS.B  8

ul_b:
   DS.B  8
