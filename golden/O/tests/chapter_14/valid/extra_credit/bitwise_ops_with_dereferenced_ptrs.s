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
__local_main__8	EQU	$88
__local_main__12	EQU	$8C
__local_main__13	EQU	$8D
__local_main__19	EQU	$93
__local_main__21	EQU	$95

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__ui,__local_main__27,__local_main__28,__local_main__29,__local_main__ul,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__i,__local_main__39 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$80
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$FF
   LDA   #$00
   BNE   .jcmp_skip@0
   TYA
   CMP   #$80
   BNE   .jcmp_skip@0
   ORA   #$00
   BNE   .jcmp_skip@0
   CMP   #$FF
   BNE   .jcmp_skip@0
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   ORA   #$00
   BMI   .sx_done@2
   LDA   #$00
.sx_done@2:
   STA   __local_main__21
   STA   __local_main__19
   LDY   #$00
   AND   #$80
   STA   __local_main__12
   TYA
   BNE   .jcmp_skip@3
   LDA   __local_main__12
   CMP   #$80
   BNE   .jcmp_skip@3
   TYA
   BNE   .jcmp_skip@3
   BEQ   .if_end@2
.jcmp_skip@3:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__21
   STA   __local_main__13
   ORA   #$80
   STA   __local_main__12
   LDA   __local_main__19
   STA   __local_main__8
   LDA   #$FF
   LDY   #$FF
   STY   __local_main__6
   BMI   .sx_neg@4
   LDA   #$00
.sx_neg@4:
   STA   __local_main__5
   STA   __local_main__4
   STA   __local_main__3
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__8
   CMP   __local_main__0
   BNE   .jcmp_skip@6
   CMP   __local_main__1
   BNE   .jcmp_skip@6
   CMP   __local_main__2
   BNE   .jcmp_skip@6
   CMP   __local_main__3
   BNE   .jcmp_skip@6
   LDA   __local_main__12
   CMP   __local_main__4
   BNE   .jcmp_skip@6
   LDA   __local_main__13
   CMP   __local_main__5
   BNE   .jcmp_skip@6
   TYA
   CMP   __local_main__6
   BNE   .jcmp_skip@6
   CMP   #$FF
   BEQ   .if_end@3
.jcmp_skip@6:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
