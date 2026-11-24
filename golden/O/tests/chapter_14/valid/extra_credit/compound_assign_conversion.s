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
__local_main__d	EQU	$8A
__local_main__i	EQU	$92
__local_main__ul	EQU	$94

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__d,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__i,__local_main__19,__local_main__ul,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$14
   STA   __local_main__d+6
   LDA   #$40
   STA   __local_main__d+7
   LDA   #<__local_main__d
   STA   __local_main__8
   LDA   #>__local_main__d
   STA   __local_main__9
   LDA   #$00
   STA   __local_main__d
   STA   __local_main__d+1
   STA   __local_main__d+2
   STA   __local_main__d+3
   STA   __local_main__d+4
   STA   __local_main__d+5
   JSR   main@run0
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$14
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   TYA
   STA   HARGS+8
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$6D
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
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
   LDA   HARGS+16
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
   LDA   __local_main__d+7
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   __local_main__d+6
   CMP   #$B3
   BNE   .jcmp_skip@0
   LDA   __local_main__d+5
   CMP   #$88
   BNE   .jcmp_skip@0
   LDA   __local_main__d+4
   BNE   .jcmp_skip@0
   LDA   __local_main__d+3
   BNE   .jcmp_skip@0
   LDA   __local_main__d+2
   BNE   .jcmp_skip@0
   LDA   __local_main__d+1
   BNE   .jcmp_skip@0
   LDA   __local_main__d
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$CE
   STA   __local_main__i
   LDA   #<__local_main__i
   STA   __local_main__6
   LDA   #>__local_main__i
   STA   __local_main__7
   LDA   #$FF
   STA   __local_main__i+1
   BMI   .sx_done@2
   LDA   #$00
.sx_done@2:
   STA   HARGS+2
   STA   HARGS+3
   LDA   #$CE
   STA   HARGS
   LDA   #$FF
   STA   HARGS+1
   LDY   #$9C
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+13
   LDY   #$01
   STA   (__local_main__6),Y
   LDA   HARGS+12
   DEY
   STA   (__local_main__6),Y
   LDA   (__local_main__6),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__6),Y
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$32
   BEQ   .if_end@1
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDY   #$00
   LDA   (__local_main__8),Y
   STA   HARGS+8
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+9
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+10
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+11
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+12
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+13
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+14
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+15
   LDA   #$00
   JSR   main@run0
   STA   HARGS+3
   LDA   #$E0
   STA   HARGS+4
   LDA   #$FF
   STA   HARGS+5
   LDA   #$EF
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   JSR   ddiv
   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   STA   HARGS+4
   LDA   HARGS+21
   STA   HARGS+5
   LDA   HARGS+22
   STA   HARGS+6
   LDA   HARGS+23
   STA   HARGS+7
   JSR   d2ul
   LDA   HARGS+11
   BNE   .jcmp_skip@4
   LDA   HARGS+10
   BNE   .jcmp_skip@4
   LDA   HARGS+9
   BNE   .jcmp_skip@4
   LDA   HARGS+8
   CMP   #$0D
   BEQ   .if_end@2
.jcmp_skip@4:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$F6
   STA   __local_main__i
   LDA   #$FF
   STA   __local_main__i+1
   STA   __local_main__ul
   STA   __local_main__ul+1
   STA   __local_main__ul+2
   LDA   #$7F
   STA   __local_main__ul+3
   LDA   #$00
   STA   __local_main__ul+7
   LDY   #$00
   LDA   (__local_main__6),Y
   SEC
   SBC   #$FF
   STA   __local_main__1
   INY
   LDA   (__local_main__6),Y
   SBC   #$FF
   STA   (__local_main__6),Y
   LDA   __local_main__1
   DEY
   STA   (__local_main__6),Y
   LDA   __local_main__i+1
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __local_main__i
   CMP   #$F7
   BEQ   .if_end@3
.jcmp_skip@5:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__ul+7
   BNE   .jcmp_skip@6
   LDA   __local_main__ul+3
   CMP   #$7F
   BNE   .jcmp_skip@6
   LDA   __local_main__ul+2
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __local_main__ul+1
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __local_main__ul
   CMP   #$FF
   BEQ   .if_end@4
.jcmp_skip@6:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   RTS
