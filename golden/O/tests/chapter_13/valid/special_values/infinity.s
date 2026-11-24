SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87
__local_main__8	EQU	$88
__local_main__9	EQU	$89
__local_main__10	EQU	$8A
__local_main__11	EQU	$8B
__local_main__12	EQU	$8C
__local_main__13	EQU	$8D
__local_main__14	EQU	$8E
__local_main__15	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item inf section=.data local=false zero=false
; item very_large section=.data local=false zero=false
; item zero section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   inf+7
   CMP   #$7F
   BNE   .jcmp_skip@0
   LDA   inf+6
   CMP   #$F0
   BNE   .jcmp_skip@0
   LDA   inf+5
   BNE   .jcmp_skip@0
   LDA   inf+4
   BNE   .jcmp_skip@0
   LDA   inf+3
   BNE   .jcmp_skip@0
   LDA   inf+2
   BNE   .jcmp_skip@0
   LDA   inf+1
   BNE   .jcmp_skip@0
   LDA   inf
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   main@run1
   LDA   inf
   STA   HARGS+8
   LDA   inf+1
   STA   HARGS+9
   LDA   inf+2
   STA   HARGS+10
   LDA   inf+3
   STA   HARGS+11
   LDA   inf+4
   STA   HARGS+12
   LDA   inf+5
   STA   HARGS+13
   LDA   inf+6
   STA   HARGS+14
   LDA   inf+7
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   main@run1
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$24
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+23
   CMP   inf+7
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   CMP   inf+6
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   CMP   inf+5
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   CMP   inf+4
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   CMP   inf+3
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   CMP   inf+2
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   CMP   inf+1
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   CMP   inf
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   main@run2
   LDA   #$3F
   JSR   main@run0
   LDA   HARGS+23
   CMP   inf+7
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   CMP   inf+6
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   CMP   inf+5
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   CMP   inf+4
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   CMP   inf+3
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   CMP   inf+2
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   CMP   inf+1
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   CMP   inf
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   inf
   STA   __local_main__15
   LDA   inf+1
   STA   __local_main__14
   LDA   inf+2
   STA   __local_main__13
   LDA   inf+3
   STA   __local_main__12
   LDA   inf+4
   STA   __local_main__11
   LDA   inf+5
   STA   __local_main__10
   LDA   inf+6
   STA   __local_main__9
   LDA   inf+7
   EOR   #$80
   STA   __local_main__8
   JSR   main@run2
   LDA   #$BF
   JSR   main@run0
   LDA   HARGS+16
   STA   __local_main__7
   LDA   HARGS+17
   STA   __local_main__6
   LDA   HARGS+18
   STA   __local_main__5
   LDA   HARGS+19
   STA   __local_main__4
   LDA   HARGS+20
   STA   __local_main__3
   LDA   HARGS+21
   STA   __local_main__2
   LDA   HARGS+22
   STA   __local_main__1
   LDA   HARGS+23
   STA   __local_main__0
   LDA   very_large
   STA   HARGS+8
   LDA   very_large+1
   STA   HARGS+9
   LDA   very_large+2
   STA   HARGS+10
   LDA   very_large+3
   STA   HARGS+11
   LDA   very_large+4
   STA   HARGS+12
   LDA   very_large+5
   STA   HARGS+13
   LDA   very_large+6
   STA   HARGS+14
   LDA   very_large+7
   EOR   #$80
   STA   HARGS+15
   LDA   __local_main__15
   STA   HARGS
   LDA   __local_main__14
   STA   HARGS+1
   LDA   __local_main__13
   STA   HARGS+2
   LDA   __local_main__12
   STA   HARGS+3
   LDA   __local_main__11
   STA   HARGS+4
   LDA   __local_main__10
   STA   HARGS+5
   LDA   __local_main__9
   STA   HARGS+6
   LDA   __local_main__8
   STA   HARGS+7
   JSR   dlt
   LDA   HARGS+16
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __local_main__8
   CMP   __local_main__0
   BNE   .jcmp_skip@3
   LDA   __local_main__9
   CMP   __local_main__1
   BNE   .jcmp_skip@3
   LDA   __local_main__10
   CMP   __local_main__2
   BNE   .jcmp_skip@3
   LDA   __local_main__11
   CMP   __local_main__3
   BNE   .jcmp_skip@3
   LDA   __local_main__12
   CMP   __local_main__4
   BNE   .jcmp_skip@3
   LDA   __local_main__13
   CMP   __local_main__5
   BNE   .jcmp_skip@3
   LDA   __local_main__14
   CMP   __local_main__6
   BNE   .jcmp_skip@3
   LDA   __local_main__15
   CMP   __local_main__7
   BEQ   .if_end@5
.jcmp_skip@3:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

main@run2:
   SUBROUTINE

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$F0
   STA   HARGS+6
   RTS

main@run1:
   SUBROUTINE

   LDA   very_large
   STA   HARGS
   LDA   very_large+1
   STA   HARGS+1
   LDA   very_large+2
   STA   HARGS+2
   LDA   very_large+3
   STA   HARGS+3
   LDA   very_large+4
   STA   HARGS+4
   LDA   very_large+5
   STA   HARGS+5
   LDA   very_large+6
   STA   HARGS+6
   LDA   very_large+7
   STA   HARGS+7
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS+7
   LDA   zero
   STA   HARGS+8
   LDA   zero+1
   STA   HARGS+9
   LDA   zero+2
   STA   HARGS+10
   LDA   zero+3
   STA   HARGS+11
   LDA   zero+4
   STA   HARGS+12
   LDA   zero+5
   STA   HARGS+13
   LDA   zero+6
   STA   HARGS+14
   LDA   zero+7
   STA   HARGS+15
   JMP   ddiv

inf:
   DC.L  $00000000
   DC.L  $7FF00000

very_large:
   DC.L  $58ADBB99
   DC.L  $7FEFDCF1

zero:
   DS.B  8
