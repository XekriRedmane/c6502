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
__local_main__i	EQU	$9B

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__o_12,__local_main__o_11,__local_main__o_10,__local_main__o_8,__local_main__o_9,__local_main__o_7,__local_main__o_6,__local_main__o_5,__local_main__o_4,__local_main__o_3,__local_main__o_2,__local_main__o_1,__local_main__o_0,__local_main__i,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=17
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$61
   STA   __local_main__i
   LDA   #$00
   LDY   #$80
   STY   __local_main__4
   LDY   #$00
   STY   __local_main__3
   STY   __local_main__2
   STY   __local_main__1
   CMP   #$00
   BNE   .jcmp_skip@0
   TYA
   BNE   .jcmp_skip@0
   LDA   __local_main__4
   CMP   #$80
   BNE   .jcmp_skip@0
   TYA
   BNE   .jcmp_skip@0
   LDA   #$63
   CMP   #$63
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   TYA
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   __local_main__13
   STA   __local_main__12
   STA   __local_main__11
   STA   __local_main__10
   STA   __local_main__9
   STA   __local_main__8
   TYA
   STA   __local_main__4
   STA   __local_main__0
   LDA   #$64
   EOR   #$A0
   STA   __local_main__7
   TYA
   EOR   #$86
   STA   __local_main__6
   LDA   __local_main__13
   EOR   #$01
   STA   __local_main__5
   LDA   __local_main__12
   EOR   __local_main__4
   STA   __local_main__4
   LDA   __local_main__11
   EOR   __local_main__3
   STA   __local_main__3
   LDA   __local_main__10
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__9
   EOR   __local_main__1
   STA   __local_main__1
   LDA   __local_main__8
   EOR   __local_main__0
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   BNE   .jcmp_skip@3
   LDA   __local_main__2
   BNE   .jcmp_skip@3
   LDA   __local_main__3
   BNE   .jcmp_skip@3
   LDA   __local_main__4
   BNE   .jcmp_skip@3
   LDA   __local_main__5
   CMP   #$01
   BNE   .jcmp_skip@3
   LDA   __local_main__6
   CMP   #$86
   BNE   .jcmp_skip@3
   LDA   __local_main__7
   CMP   #$C4
   BEQ   .if_end@2
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDA   #$02
   STA   HARGS
   TYA
   STA   HARGS+1
   STA   HARGS+2
   LDY   #$80
   STY   HARGS+3
   LDY   #$00
   JSR   main@run0
   LDA   #$1A
   STA   HARGS+8
   JSR   lsr64
   LDA   HARGS+13
   BNE   .jcmp_skip@4
   LDA   HARGS+12
   BNE   .jcmp_skip@4
   LDA   HARGS+11
   BNE   .jcmp_skip@4
   LDA   HARGS+10
   BNE   .jcmp_skip@4
   LDA   HARGS+9
   CMP   #$20
   BEQ   .if_end@3
.jcmp_skip@4:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$0C
   STA   HARGS+8
   LDA   __local_main__i
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STY   HARGS+3
   JSR   main@run0
   JSR   asl64
   LDA   HARGS+16
   BNE   .jcmp_skip@5
   LDA   HARGS+15
   BNE   .jcmp_skip@5
   LDA   HARGS+14
   BNE   .jcmp_skip@5
   LDA   HARGS+13
   BNE   .jcmp_skip@5
   LDA   HARGS+12
   BNE   .jcmp_skip@5
   LDA   HARGS+11
   CMP   #$06
   BNE   .jcmp_skip@5
   LDA   HARGS+10
   CMP   #$10
   BNE   .jcmp_skip@5
   LDA   HARGS+9
   BEQ   .if_end@4
.jcmp_skip@5:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   RTS
