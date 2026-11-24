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
__local_addition__4	EQU	$86
__local_complement__4	EQU	$86
__local_subtraction__4	EQU	$86
__local_addition__5	EQU	$87
__local_complement__5	EQU	$87
__local_subtraction__5	EQU	$87
__local_addition__6	EQU	$88
__local_complement__6	EQU	$88
__local_subtraction__6	EQU	$88
__local_addition__7	EQU	$89
__local_complement__7	EQU	$89
__local_subtraction__7	EQU	$89

; @zp-link-meta-begin
; def addition params= locals=__local_addition__0,__local_addition__1,__local_addition__2,__local_addition__3,__local_addition__4,__local_addition__5,__local_addition__6,__local_addition__7 indirect=false in_cycle=false
; def complement params= locals=__local_complement__0,__local_complement__1,__local_complement__2,__local_complement__3,__local_complement__4,__local_complement__5,__local_complement__6,__local_complement__7 indirect=false in_cycle=false
; def division params= locals=__local_division__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def multiplication params= locals=__local_multiplication__0,__local_multiplication__1,__local_multiplication__2,__local_multiplication__3,__local_multiplication__4,__local_multiplication__5,__local_multiplication__6,__local_multiplication__7 indirect=false in_cycle=false
; def remaind params= locals=__local_remaind__0 indirect=false in_cycle=false
; def subtraction params= locals=__local_subtraction__0,__local_subtraction__1,__local_subtraction__2,__local_subtraction__3,__local_subtraction__4,__local_subtraction__5,__local_subtraction__6,__local_subtraction__7 indirect=false in_cycle=false
; call main -> addition
; call main -> complement
; call main -> division
; call main -> multiplication
; call main -> remaind
; call main -> subtraction
; item addition section=.text local=false zero=false
; item subtraction section=.text local=false zero=false
; item multiplication section=.text local=false zero=false
; item multiplication@run0 section=.text local=true zero=false
; item division section=.text local=false zero=false
; item division@run4 section=.text local=true zero=false
; item remaind section=.text local=false zero=false
; item complement section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run5 section=.text local=true zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item a section=.bss local=false zero=true
; item b section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=32
; @zp-link-meta-end

addition:
   SUBROUTINE

   LDA   a
   CLC
   ADC   b
   STA   __local_addition__7
   LDA   a+1
   ADC   b+1
   STA   __local_addition__6
   LDA   a+2
   ADC   b+2
   STA   __local_addition__5
   LDA   a+3
   ADC   b+3
   STA   __local_addition__4
   LDA   a+4
   ADC   b+4
   STA   __local_addition__3
   LDA   a+5
   ADC   b+5
   STA   __local_addition__2
   LDA   a+6
   ADC   b+6
   STA   __local_addition__1
   LDA   a+7
   ADC   b+7
   BNE   .thread@0
   LDA   __local_addition__1
   BNE   .thread@0
   LDA   __local_addition__2
   BNE   .thread@0
   LDA   __local_addition__3
   BNE   .thread@0
   LDA   __local_addition__4
   CMP   #$7F
   BNE   .thread@0
   LDA   __local_addition__5
   CMP   #$FF
   BNE   .thread@0
   LDA   __local_addition__6
   CMP   #$FF
   BNE   .thread@0
   LDA   __local_addition__7
   CMP   #$FD
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

   LDA   a
   SEC
   SBC   b
   STA   __local_subtraction__7
   LDA   a+1
   SBC   b+1
   STA   __local_subtraction__6
   LDA   a+2
   SBC   b+2
   STA   __local_subtraction__5
   LDA   a+3
   SBC   b+3
   STA   __local_subtraction__4
   LDA   a+4
   SBC   b+4
   STA   __local_subtraction__3
   LDA   a+5
   SBC   b+5
   STA   __local_subtraction__2
   LDA   a+6
   SBC   b+6
   STA   __local_subtraction__1
   LDA   a+7
   SBC   b+7
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__1
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__2
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__3
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__4
   CMP   #$88
   BNE   .thread@1
   LDA   __local_subtraction__5
   CMP   #$CA
   BNE   .thread@1
   LDA   __local_subtraction__6
   CMP   #$6B
   BNE   .thread@1
   LDA   __local_subtraction__7
   CMP   #$A6
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
   LDA   #$02
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+16
   BNE   .thread@2
   LDA   HARGS+15
   BNE   .thread@2
   LDA   HARGS+14
   BNE   .thread@2
   LDA   HARGS+13
   BNE   .thread@2
   LDA   HARGS+12
   CMP   #$77
   BNE   .thread@2
   LDA   HARGS+11
   CMP   #$35
   BNE   .thread@2
   LDA   HARGS+10
   CMP   #$94
   BNE   .thread@2
   LDA   HARGS+9
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

   LDA   a
   STA   HARGS
   LDA   a+1
   STA   HARGS+1
   LDA   a+2
   STA   HARGS+2
   LDA   a+3
   STA   HARGS+3
   LDA   a+4
   STA   HARGS+4
   LDA   a+5
   STA   HARGS+5
   LDA   a+6
   STA   HARGS+6
   LDA   a+7
   STA   HARGS+7
   RTS

division:
   SUBROUTINE

   JSR   multiplication@run0
   LDA   #$80
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   JSR   division@run4
   LDA   HARGS+16
   STA   b
   LDA   HARGS+17
   STA   b+1
   LDA   HARGS+18
   STA   b+2
   LDA   HARGS+19
   STA   b+3
   LDA   HARGS+20
   STA   b+4
   LDA   HARGS+21
   STA   b+5
   LDA   HARGS+22
   STA   b+6
   LDA   HARGS+23
   STA   b+7
   BNE   .thread@3
   LDA   b+6
   BNE   .thread@3
   LDA   b+5
   BNE   .thread@3
   LDA   b+4
   BNE   .thread@3
   LDA   b+3
   BNE   .thread@3
   LDA   b+2
   CMP   #$0B
   BNE   .thread@3
   LDA   b+1
   CMP   #$EB
   BNE   .thread@3
   LDA   b
   CMP   #$C2
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
   RTS

division@run4:
   SUBROUTINE

   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JMP   sdivmod64

remaind:
   SUBROUTINE

   SEC
   LDA   #$00
   SBC   a
   STA   HARGS
   LDA   #$00
   SBC   a+1
   STA   HARGS+1
   LDA   #$00
   SBC   a+2
   STA   HARGS+2
   LDA   #$00
   SBC   a+3
   STA   HARGS+3
   LDA   #$00
   SBC   a+4
   STA   HARGS+4
   LDA   #$00
   SBC   a+5
   STA   HARGS+5
   LDA   #$00
   SBC   a+6
   STA   HARGS+6
   LDA   #$00
   SBC   a+7
   STA   HARGS+7
   LDA   #$CA
   STA   HARGS+9
   LDA   #$9A
   STA   HARGS+10
   LDA   #$3B
   STA   HARGS+11
   LDA   #$00
   STA   HARGS+8
   STA   HARGS+12
   LDY   #$00
   JSR   division@run4
   LDA   HARGS+24
   STA   b
   LDA   HARGS+25
   STA   b+1
   LDA   HARGS+26
   STA   b+2
   LDA   HARGS+27
   STA   b+3
   LDA   HARGS+28
   STA   b+4
   LDA   HARGS+29
   STA   b+5
   LDA   HARGS+30
   STA   b+6
   LDA   HARGS+31
   STA   b+7
   CMP   #$FF
   BNE   .thread@4
   LDA   b+6
   CMP   #$FF
   BNE   .thread@4
   LDA   b+5
   CMP   #$FF
   BNE   .thread@4
   LDA   b+4
   CMP   #$FF
   BNE   .thread@4
   LDA   b+3
   CMP   #$FF
   BNE   .thread@4
   LDA   b+2
   CMP   #$FF
   BNE   .thread@4
   LDA   b+1
   CMP   #$FF
   BNE   .thread@4
   LDA   b
   CMP   #$FB
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDX   #$00
   RTS

complement:
   SUBROUTINE

   LDA   a
   EOR   #$FF
   STA   __local_complement__7
   LDA   a+1
   EOR   #$FF
   STA   __local_complement__6
   LDA   a+2
   EOR   #$FF
   STA   __local_complement__5
   LDA   a+3
   EOR   #$FF
   STA   __local_complement__4
   LDA   a+4
   EOR   #$FF
   STA   __local_complement__3
   LDA   a+5
   EOR   #$FF
   STA   __local_complement__2
   LDA   a+6
   EOR   #$FF
   STA   __local_complement__1
   LDA   a+7
   EOR   #$FF
   CMP   #$FF
   BNE   .thread@5
   LDA   __local_complement__1
   CMP   #$FF
   BNE   .thread@5
   LDA   __local_complement__2
   CMP   #$FF
   BNE   .thread@5
   LDA   __local_complement__3
   CMP   #$FF
   BNE   .thread@5
   LDA   __local_complement__4
   CMP   #$80
   BNE   .thread@5
   LDA   __local_complement__5
   BNE   .thread@5
   LDA   __local_complement__6
   BNE   .thread@5
   LDA   __local_complement__7
   CMP   #$01
   BEQ   .cmp_true@15
.thread@5:
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$F8
   STA   a
   LDA   #$FF
   STA   a+1
   STA   a+2
   LDA   #$7F
   STA   a+3
   LDA   #$05
   STA   b
   LDA   #$00
   STA   a+4
   JSR   main@run1
   JSR   main@run2
   JSR   addition
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$6C
   STA   a+1
   LDA   #$CA
   STA   a+2
   LDA   #$88
   STA   a+3
   LDA   #$FF
   STA   a+4
   JSR   main@run1
   LDA   #$5A
   STA   b
   LDA   #$00
   STA   a
   JSR   main@run2
   JSR   subtraction
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$65
   STA   a+1
   LDA   #$CD
   STA   a+2
   LDA   #$1D
   JSR   main@run3
   JSR   multiplication
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$E1
   STA   a+1
   LDA   #$F5
   STA   a+2
   LDA   #$05
   JSR   main@run3
   JSR   division
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$05
   STA   a
   LDA   #$CA
   STA   a+1
   LDA   #$9A
   STA   a+2
   LDA   #$3B
   STA   a+3
   JSR   main@run5
   JSR   remaind
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$FE
   STA   a
   LDA   #$FF
   STA   a+1
   STA   a+2
   LDA   #$7F
   STA   a+3
   JSR   main@run5
   JSR   complement
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

main@run5:
   SUBROUTINE

   LDA   #$00
   STA   a+4
   JMP   main@run1

main@run3:
   SUBROUTINE

   STA   a+3
   LDA   #$00
   STA   a
   STA   a+4
   JMP   main@run1

main@run2:
   SUBROUTINE

   STA   b+1
   STA   b+2
   STA   b+3
   STA   b+4
   STA   b+5
   STA   b+6
   STA   b+7
   RTS

main@run1:
   SUBROUTINE

   STA   a+5
   STA   a+6
   STA   a+7
   RTS

a:
   DS.B  8

b:
   DS.B  8
