SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
@0.d	EQU	$26
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp @0.d size=8 at=auto def=true init=$00,$00,$00,$00,$00,$00,$E8,$3F weight=25 local=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   @0.d
   STA   __local_main__7
   STA   HARGS
   LDA   @0.d+1
   STA   __local_main__6
   STA   HARGS+1
   LDA   @0.d+2
   STA   __local_main__5
   STA   HARGS+2
   LDA   @0.d+3
   STA   __local_main__4
   STA   HARGS+3
   LDA   @0.d+4
   STA   __local_main__3
   STA   HARGS+4
   LDA   @0.d+5
   STA   __local_main__2
   STA   HARGS+5
   LDA   @0.d+6
   STA   __local_main__1
   STA   HARGS+6
   LDA   @0.d+7
   STA   __local_main__0
   JSR   main@run2
   LDA   __local_main__0
   CMP   #$3F
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$E8
   BNE   .jcmp_skip@0
   LDA   __local_main__2
   BNE   .jcmp_skip@0
   LDA   __local_main__3
   BNE   .jcmp_skip@0
   LDA   __local_main__4
   BNE   .jcmp_skip@0
   LDA   __local_main__5
   BNE   .jcmp_skip@0
   LDA   __local_main__6
   BNE   .jcmp_skip@0
   LDA   __local_main__7
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   @0.d+7
   CMP   #$3F
   BNE   .jcmp_skip@1
   LDA   @0.d+6
   CMP   #$FC
   BNE   .jcmp_skip@1
   LDA   @0.d+5
   BNE   .jcmp_skip@1
   LDA   @0.d+4
   BNE   .jcmp_skip@1
   LDA   @0.d+3
   BNE   .jcmp_skip@1
   LDA   @0.d+2
   BNE   .jcmp_skip@1
   LDA   @0.d+1
   BNE   .jcmp_skip@1
   LDA   @0.d
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$CD
   STA   @0.d
   STA   HARGS
   LDA   #$0C
   STA   @0.d+5
   LDA   #$59
   STA   @0.d+6
   LDA   #$C0
   STA   @0.d+7
   LDA   #$CC
   STA   @0.d+1
   STA   @0.d+2
   STA   @0.d+3
   STA   @0.d+4
   STA   HARGS+1
   LDY   #$CC
   STY   HARGS+2
   STY   HARGS+3
   STA   HARGS+4
   LDA   #$0C
   STA   HARGS+5
   LDA   #$59
   STA   HARGS+6
   LDA   #$C0
   JSR   main@run2
   CMP   #$C0
   BNE   .jcmp_skip@2
   LDA   @0.d+6
   CMP   #$58
   BNE   .jcmp_skip@2
   LDA   @0.d+5
   CMP   #$CC
   BNE   .jcmp_skip@2
   LDA   @0.d+4
   CMP   #$CC
   BNE   .jcmp_skip@2
   LDA   @0.d+3
   CMP   #$CC
   BNE   .jcmp_skip@2
   LDA   @0.d+2
   CMP   #$CC
   BNE   .jcmp_skip@2
   LDA   @0.d+1
   CMP   #$CC
   BNE   .jcmp_skip@2
   LDA   @0.d
   CMP   #$CD
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   @0.d+7
   CMP   #$C0
   BNE   .jcmp_skip@3
   LDA   @0.d+6
   CMP   #$58
   BNE   .jcmp_skip@3
   LDA   @0.d+5
   CMP   #$CC
   BNE   .jcmp_skip@3
   LDA   @0.d+4
   CMP   #$CC
   BNE   .jcmp_skip@3
   LDA   @0.d+3
   CMP   #$CC
   BNE   .jcmp_skip@3
   LDA   @0.d+2
   CMP   #$CC
   BNE   .jcmp_skip@3
   LDA   @0.d+1
   CMP   #$CC
   BNE   .jcmp_skip@3
   LDA   @0.d
   CMP   #$CD
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   STA   __local_main__7
   LDA   @0.d+1
   STA   __local_main__6
   STA   HARGS+1
   LDA   @0.d+2
   STA   __local_main__5
   STA   HARGS+2
   LDA   @0.d+3
   STA   __local_main__4
   STA   HARGS+3
   LDA   @0.d+4
   STA   __local_main__3
   STA   HARGS+4
   LDA   @0.d+5
   STA   __local_main__2
   STA   HARGS+5
   LDA   @0.d+6
   STA   __local_main__1
   STA   HARGS+6
   LDA   @0.d+7
   STA   __local_main__0
   STA   HARGS+7
   LDA   @0.d
   STA   HARGS
   JSR   main@run3
   LDA   __local_main__0
   CMP   #$C0
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$58
   BNE   .jcmp_skip@4
   LDA   __local_main__2
   CMP   #$CC
   BNE   .jcmp_skip@4
   LDA   __local_main__3
   CMP   #$CC
   BNE   .jcmp_skip@4
   LDA   __local_main__4
   CMP   #$CC
   BNE   .jcmp_skip@4
   LDA   __local_main__5
   CMP   #$CC
   BNE   .jcmp_skip@4
   LDA   __local_main__6
   CMP   #$CC
   BNE   .jcmp_skip@4
   LDA   __local_main__7
   CMP   #$CD
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   @0.d+7
   CMP   #$C0
   BNE   .jcmp_skip@5
   LDA   @0.d+6
   CMP   #$59
   BNE   .jcmp_skip@5
   LDA   @0.d+5
   CMP   #$0C
   BNE   .jcmp_skip@5
   LDA   @0.d+4
   CMP   #$CC
   BNE   .jcmp_skip@5
   LDA   @0.d+3
   CMP   #$CC
   BNE   .jcmp_skip@5
   LDA   @0.d+2
   CMP   #$CC
   BNE   .jcmp_skip@5
   LDA   @0.d+1
   CMP   #$CC
   BNE   .jcmp_skip@5
   LDA   @0.d
   CMP   #$CD
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   STA   HARGS
   LDA   @0.d+1
   STA   HARGS+1
   LDA   @0.d+2
   STA   HARGS+2
   LDA   @0.d+3
   STA   HARGS+3
   LDA   @0.d+4
   STA   HARGS+4
   LDA   @0.d+5
   STA   HARGS+5
   LDA   @0.d+6
   STA   HARGS+6
   LDA   @0.d+7
   STA   HARGS+7
   JSR   main@run3
   CMP   #$C0
   BNE   .jcmp_skip@6
   LDA   @0.d+6
   CMP   #$59
   BNE   .jcmp_skip@6
   LDA   @0.d+5
   CMP   #$4C
   BNE   .jcmp_skip@6
   LDA   @0.d+4
   CMP   #$CC
   BNE   .jcmp_skip@6
   LDA   @0.d+3
   CMP   #$CC
   BNE   .jcmp_skip@6
   LDA   @0.d+2
   CMP   #$CC
   BNE   .jcmp_skip@6
   LDA   @0.d+1
   CMP   #$CC
   BNE   .jcmp_skip@6
   LDA   @0.d
   CMP   #$CD
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   @0.d+7
   CMP   #$C0
   BNE   .jcmp_skip@7
   LDA   @0.d+6
   CMP   #$59
   BNE   .jcmp_skip@7
   LDA   @0.d+5
   CMP   #$4C
   BNE   .jcmp_skip@7
   LDA   @0.d+4
   CMP   #$CC
   BNE   .jcmp_skip@7
   LDA   @0.d+3
   CMP   #$CC
   BNE   .jcmp_skip@7
   LDA   @0.d+2
   CMP   #$CC
   BNE   .jcmp_skip@7
   LDA   @0.d+1
   CMP   #$CC
   BNE   .jcmp_skip@7
   LDA   @0.d
   CMP   #$CD
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   #$4F
   STA   @0.d
   STA   HARGS
   LDA   #$9B
   STA   @0.d+1
   STA   HARGS+1
   LDA   #$0E
   STA   @0.d+2
   STA   HARGS+2
   LDA   #$0A
   STA   @0.d+3
   STA   HARGS+3
   LDA   #$B4
   STA   @0.d+4
   STA   HARGS+4
   LDA   #$E3
   STA   @0.d+5
   STA   HARGS+5
   LDA   #$92
   STA   @0.d+6
   STA   HARGS+6
   LDA   #$3B
   STA   @0.d+7
   JSR   main@run2
   CMP   #$3F
   BNE   .jcmp_skip@8
   LDA   @0.d+6
   CMP   #$F0
   BNE   .jcmp_skip@8
   LDA   @0.d+5
   BNE   .jcmp_skip@8
   LDA   @0.d+4
   BNE   .jcmp_skip@8
   LDA   @0.d+3
   BNE   .jcmp_skip@8
   LDA   @0.d+2
   BNE   .jcmp_skip@8
   LDA   @0.d+1
   BNE   .jcmp_skip@8
   LDA   @0.d
   BEQ   .if_end@8
.jcmp_skip@8:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   #$50
   STA   @0.d
   STA   HARGS
   LDA   #$EF
   STA   @0.d+1
   STA   HARGS+1
   LDA   #$E2
   STA   @0.d+2
   STA   HARGS+2
   LDA   #$D6
   STA   @0.d+3
   STA   HARGS+3
   LDA   #$E4
   STA   @0.d+4
   STA   HARGS+4
   LDA   #$1A
   STA   @0.d+5
   STA   HARGS+5
   LDA   #$4B
   STA   @0.d+6
   STA   HARGS+6
   LDA   #$44
   STA   @0.d+7
   STA   HARGS+7
   JSR   main@run3
   CMP   #$44
   BNE   .jcmp_skip@9
   LDA   @0.d+6
   CMP   #$4B
   BNE   .jcmp_skip@9
   LDA   @0.d+5
   CMP   #$1A
   BNE   .jcmp_skip@9
   LDA   @0.d+4
   CMP   #$E4
   BNE   .jcmp_skip@9
   LDA   @0.d+3
   CMP   #$D6
   BNE   .jcmp_skip@9
   LDA   @0.d+2
   CMP   #$E2
   BNE   .jcmp_skip@9
   LDA   @0.d+1
   CMP   #$EF
   BNE   .jcmp_skip@9
   LDA   @0.d
   CMP   #$50
   BEQ   .if_end@9
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDX   #$00
   TXA
   RTS

main@run3:
   SUBROUTINE

   JSR   main@run1
   JSR   dsub
   JMP   main@run0

main@run2:
   SUBROUTINE

   STA   HARGS+7
   JSR   main@run1
   JSR   dadd
   JMP   main@run0

main@run1:
   SUBROUTINE

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

main@run0:
   SUBROUTINE

   LDA   HARGS+16
   STA   @0.d
   LDA   HARGS+17
   STA   @0.d+1
   LDA   HARGS+18
   STA   @0.d+2
   LDA   HARGS+19
   STA   @0.d+3
   LDA   HARGS+20
   STA   @0.d+4
   LDA   HARGS+21
   STA   @0.d+5
   LDA   HARGS+22
   STA   @0.d+6
   LDA   HARGS+23
   STA   @0.d+7
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   @0.d
   STA   @0.d+1
   STA   @0.d+2
   STA   @0.d+3
   STA   @0.d+4
   STA   @0.d+5
   LDA   #$E8
   STA   @0.d+6
   LDA   #$3F
   STA   @0.d+7
   RTS
