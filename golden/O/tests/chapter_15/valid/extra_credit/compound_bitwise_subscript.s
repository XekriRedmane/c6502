SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__arr_11	EQU	$84
__local_main__arr_10	EQU	$85
__local_main__arr_9	EQU	$86
__local_main__arr_8	EQU	$87
__local_main__0	EQU	$88
__local_main__1	EQU	$89
__local_main__2	EQU	$8A
__local_main__3	EQU	$8B
__local_main__4	EQU	$8C
__local_main__5	EQU	$8D
__local_main__6	EQU	$8E
__local_main__7	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__arr_15,__local_main__arr_14,__local_main__arr_13,__local_main__arr_12,__local_main__arr_11,__local_main__arr_10,__local_main__arr_9,__local_main__arr_8,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__arr_31,__local_main__arr_30,__local_main__arr_29,__local_main__arr_28,__local_main__arr_27,__local_main__arr_26,__local_main__arr_25,__local_main__arr_24,__local_main__arr_23,__local_main__arr_22,__local_main__arr_21,__local_main__arr_20,__local_main__arr_19,__local_main__arr_18,__local_main__arr_17,__local_main__arr_16,__local_main__arr_7,__local_main__arr_6,__local_main__arr_5,__local_main__arr_4,__local_main__arr_3,__local_main__arr_2,__local_main__arr_1,__local_main__arr_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=17
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   LDY   #$00
   AND   #$0F
   STA   __local_main__7
   TYA
   AND   #$0F
   STA   __local_main__6
   LDA   #$0F
   STA   __local_main__5
   STA   __local_main__4
   TYA
   STA   __local_main__3
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   BNE   .jcmp_skip@0
   LDA   #$0F
   CMP   #$0F
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
   TYA
   ORA   __local_main__7
   STA   __local_main__arr_8
   LDA   #$80
   ORA   __local_main__6
   STA   __local_main__arr_9
   TYA
   ORA   #$0F
   STA   __local_main__arr_10
   TYA
   ORA   #$0F
   STA   __local_main__arr_11
   TYA
   BNE   .jcmp_skip@1
   LDA   __local_main__arr_11
   CMP   #$0F
   BNE   .jcmp_skip@1
   LDA   __local_main__arr_10
   CMP   #$0F
   BNE   .jcmp_skip@1
   LDA   __local_main__arr_9
   CMP   #$80
   BNE   .jcmp_skip@1
   LDA   __local_main__arr_8
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   TYA
   EOR   #$0F
   STA   __local_main__arr_8
   TYA
   EOR   #$0F
   STA   __local_main__arr_9
   TYA
   EOR   #$0F
   STA   __local_main__arr_10
   LDA   #$80
   EOR   #$0F
   STA   __local_main__arr_11
   TYA
   BNE   .jcmp_skip@2
   LDA   __local_main__arr_11
   CMP   #$8F
   BNE   .jcmp_skip@2
   LDA   __local_main__arr_10
   CMP   #$0F
   BNE   .jcmp_skip@2
   LDA   __local_main__arr_9
   CMP   #$0F
   BNE   .jcmp_skip@2
   LDA   __local_main__arr_8
   CMP   #$0F
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$0F
   STA   HARGS
   LDY   #$0F
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDA   #$0C
   STA   HARGS+8
   JSR   lsr64
   LDA   HARGS+16
   BNE   .jcmp_skip@3
   LDA   HARGS+15
   BNE   .jcmp_skip@3
   LDA   HARGS+14
   BNE   .jcmp_skip@3
   LDA   HARGS+13
   BNE   .jcmp_skip@3
   LDA   HARGS+12
   BNE   .jcmp_skip@3
   LDA   HARGS+11
   BNE   .jcmp_skip@3
   LDA   HARGS+10
   CMP   #$F0
   BNE   .jcmp_skip@3
   LDA   HARGS+9
   CMP   #$F0
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
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
   STA   HARGS+7
   LDA   #$04
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+16
   BNE   .jcmp_skip@4
   LDA   HARGS+15
   BNE   .jcmp_skip@4
   LDA   HARGS+14
   BNE   .jcmp_skip@4
   LDA   HARGS+13
   BNE   .jcmp_skip@4
   LDA   HARGS+12
   CMP   #$F0
   BNE   .jcmp_skip@4
   LDA   HARGS+11
   CMP   #$F0
   BNE   .jcmp_skip@4
   LDA   HARGS+10
   BNE   .jcmp_skip@4
   LDA   HARGS+9
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS
