SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__6	EQU	$86
__local_main__ui	EQU	$8C

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__ui,__local_main__13,__local_main__14,__local_main__15,__local_main__l,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item ul section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   ul
   LDX   ul+1
   STX   __local_main__6
   LDX   ul+7
   STX   __local_main__0
   AND   #$18
   STA   ul
   LDA   __local_main__6
   AND   #$FC
   STA   ul+1
   LDA   __local_main__0
   BNE   .jcmp_skip@0
   LDA   ul+6
   BNE   .jcmp_skip@0
   LDA   ul+5
   BNE   .jcmp_skip@0
   LDA   ul+4
   BNE   .jcmp_skip@0
   LDA   ul+3
   CMP   #$FB
   BNE   .jcmp_skip@0
   LDA   ul+2
   CMP   #$FA
   BNE   .jcmp_skip@0
   LDA   ul+1
   CMP   #$F8
   BNE   .jcmp_skip@0
   LDA   ul
   CMP   #$18
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$FF
   STA   ul+1
   LDA   __local_main__0
   BNE   .jcmp_skip@1
   LDA   ul+6
   BNE   .jcmp_skip@1
   LDA   ul+5
   BNE   .jcmp_skip@1
   LDA   ul+4
   BNE   .jcmp_skip@1
   LDA   ul+3
   CMP   #$FB
   BNE   .jcmp_skip@1
   LDA   ul+2
   CMP   #$FA
   BNE   .jcmp_skip@1
   LDA   ul+1
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   ul
   CMP   #$18
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$F0
   EOR   #$F0
   STA   __local_main__3
   LDA   #$F0
   EOR   #$F0
   STA   __local_main__2
   LDA   #$00
   EOR   #$FF
   STA   __local_main__1
   LDA   #$00
   EOR   #$FF
   STA   __local_main__0
   LDA   __local_main__3
   STA   __local_main__ui
   LDA   __local_main__2
   STA   __local_main__ui+1
   LDA   __local_main__1
   STA   __local_main__ui+2
   LDA   __local_main__0
   STA   __local_main__ui+3
   LDA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__ui
   ORA   __local_main__ui+1
   ORA   __local_main__ui+2
   ORA   __local_main__ui+3
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$04
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

ul:
   DC.L  $FBFAF9F8
   DC.L  $00000000
