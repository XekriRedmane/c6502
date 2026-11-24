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
__local_main__x	EQU	$92
__local_main__ul	EQU	$94
__local_main__d	EQU	$9C

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__x,__local_main__19,__local_main__ul,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__d,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__0
   LDA   #$00
   STA   __local_main__1
   INC   __local_main__0
   BNE   .inc_done@0
   INC   __local_main__1
.inc_done@0:
   LDA   __local_main__0
   STA   __local_main__x
   LDA   __local_main__1
   STA   __local_main__x+1
   BNE   .jcmp_skip@0
   LDA   __local_main__0
   CMP   #$0B
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__x+1
   BNE   .jcmp_skip@1
   LDA   __local_main__x
   CMP   #$0B
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   ORA   #$00
   BNE   .carry_inc@0
   DEC   __local_main__1
.carry_inc@0:
   DEC   __local_main__0
   LDA   __local_main__0
   STA   __local_main__x
   LDA   __local_main__1
   STA   __local_main__x+1
   BNE   .jcmp_skip@2
   LDA   __local_main__0
   CMP   #$0A
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__x+1
   BNE   .jcmp_skip@3
   LDA   __local_main__x
   CMP   #$0A
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   STA   __local_main__3
   LDA   __local_main__x+1
   STA   __local_main__2
   LDA   __local_main__3
   CLC
   ADC   #$01
   STA   __local_main__1
   LDA   __local_main__2
   ADC   #$00
   STA   __local_main__x+1
   LDA   __local_main__1
   STA   __local_main__x
   LDA   __local_main__2
   BNE   .jcmp_skip@4
   LDA   __local_main__3
   CMP   #$0A
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __local_main__x+1
   BNE   .jcmp_skip@5
   LDA   __local_main__x
   CMP   #$0B
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   STA   __local_main__3
   LDA   __local_main__x+1
   STA   __local_main__2
   LDA   __local_main__3
   SEC
   SBC   #$01
   STA   __local_main__1
   LDA   __local_main__2
   SBC   #$00
   STA   __local_main__x+1
   LDA   __local_main__1
   STA   __local_main__x
   LDA   __local_main__2
   BNE   .jcmp_skip@6
   LDA   __local_main__3
   CMP   #$0B
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   __local_main__x+1
   BNE   .jcmp_skip@7
   LDA   __local_main__x
   CMP   #$0A
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   #$00
   LDY   #$00
   SEC
   SBC   #$01
   STA   __local_main__7
   TYA
   SBC   #$00
   STA   __local_main__6
   TYA
   SBC   #$00
   STA   __local_main__5
   TYA
   SBC   #$00
   STA   __local_main__4
   TYA
   SBC   #$00
   STA   __local_main__3
   TYA
   SBC   #$00
   STA   __local_main__2
   TYA
   SBC   #$00
   STA   __local_main__1
   TYA
   SBC   #$00
   LDX   __local_main__7
   STX   __local_main__ul
   LDX   __local_main__6
   STX   __local_main__ul+1
   LDX   __local_main__5
   STX   __local_main__ul+2
   LDX   __local_main__4
   STX   __local_main__ul+3
   LDX   __local_main__3
   STX   __local_main__ul+4
   LDX   __local_main__2
   STX   __local_main__ul+5
   LDX   __local_main__1
   STX   __local_main__ul+6
   STA   __local_main__ul+7
   TYA
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   __local_main__ul+7
   BNE   .jcmp_skip@8
   LDA   __local_main__ul+6
   BNE   .jcmp_skip@8
   LDA   __local_main__ul+5
   BNE   .jcmp_skip@8
   LDA   __local_main__ul+4
   BNE   .jcmp_skip@8
   LDA   __local_main__ul+3
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   __local_main__ul+2
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   __local_main__ul+1
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   __local_main__ul
   CMP   #$FF
   BEQ   .if_end@9
.jcmp_skip@8:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDA   #$00
   STA   __local_main__d
   STA   __local_main__d+1
   STA   __local_main__d+2
   STA   __local_main__d+3
   STA   __local_main__d+4
   STA   __local_main__d+5
   STA   __local_main__d+6
   STA   __local_main__d+7
   LDA   #<__local_main__d
   STA   __local_main__8
   LDA   #>__local_main__d
   STA   __local_main__9
   TYA
   STA   HARGS
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   STY   HARGS+8
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
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
   LDA   __local_main__7
   LDY   #$00
   STA   (__local_main__8),Y
   LDA   __local_main__6
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__5
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__4
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__3
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__2
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__0
   INY
   STA   (__local_main__8),Y
   CMP   #$3F
   BNE   .jcmp_skip@9
   LDA   __local_main__1
   CMP   #$F0
   BNE   .jcmp_skip@9
   LDA   __local_main__2
   BNE   .jcmp_skip@9
   LDA   __local_main__3
   BNE   .jcmp_skip@9
   LDA   __local_main__4
   BNE   .jcmp_skip@9
   LDA   __local_main__5
   BNE   .jcmp_skip@9
   LDA   __local_main__6
   BNE   .jcmp_skip@9
   LDA   __local_main__7
   BEQ   .if_end@10
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDA   __local_main__d+7
   CMP   #$3F
   BNE   .jcmp_skip@10
   LDA   __local_main__d+6
   CMP   #$F0
   BNE   .jcmp_skip@10
   LDA   __local_main__d+5
   BNE   .jcmp_skip@10
   LDA   __local_main__d+4
   BNE   .jcmp_skip@10
   LDA   __local_main__d+3
   BNE   .jcmp_skip@10
   LDA   __local_main__d+2
   BNE   .jcmp_skip@10
   LDA   __local_main__d+1
   BNE   .jcmp_skip@10
   LDA   __local_main__d
   BEQ   .if_end@11
.jcmp_skip@10:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDX   #$00
   TXA
   RTS
