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
__local_main__x	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__x,__local_main__9 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=8
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #<__local_main__x
   STA   __local_main__6
   LDA   #>__local_main__x
   STA   __local_main__7
   LDA   #$00
   STA   __local_main__1
   LDA   #$0A
   CLC
   ADC   #$05
   BCC   .carry_inc@0
   INC   __local_main__1
.carry_inc@0:
   STA   __local_main__x
   LDA   __local_main__1
   BNE   .jcmp_skip@0
   LDA   __local_main__x
   CMP   #$0F
   BEQ   .vthread@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.vthread@0:
   SEC
   SBC   #$0C
   STA   __local_main__0
   BCS   .carry_inc@1
   DEC   __local_main__1
.carry_inc@1:
   STA   __local_main__x
   LDA   __local_main__1
   STA   __local_main__x+1
   BNE   .jcmp_skip@1
   LDA   __local_main__0
   CMP   #$03
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__x+1
   BNE   .jcmp_skip@2
   LDA   __local_main__x
   CMP   #$03
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   STA   __local_main__4
   LDA   __local_main__x+1
   ASL   __local_main__4
   ROL
   STA   __local_main__5
   LDA   __local_main__4
   ASL
   STA   __local_main__2
   LDA   __local_main__5
   ROL
   STA   __local_main__3
   LDA   __local_main__2
   CLC
   ADC   __local_main__4
   STA   __local_main__1
   LDA   __local_main__3
   ADC   __local_main__5
   STA   __local_main__x+1
   LDA   __local_main__1
   STA   __local_main__x
   LDA   __local_main__x+1
   BNE   .jcmp_skip@3
   LDA   __local_main__x
   CMP   #$12
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   STA   HARGS
   LDA   __local_main__x+1
   STA   HARGS+1
   LDA   #$09
   JSR   main@run0
   LDA   HARGS+5
   LDY   #$01
   STA   (__local_main__6),Y
   LDA   HARGS+4
   DEY
   STA   (__local_main__6),Y
   LDA   __local_main__x+1
   BNE   .jcmp_skip@4
   LDA   __local_main__x
   CMP   #$02
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDY   #$00
   LDA   (__local_main__6),Y
   STA   HARGS
   INY
   LDA   (__local_main__6),Y
   STA   HARGS+1
   LDA   #$03
   JSR   main@run0
   LDA   HARGS+7
   LDY   #$01
   STA   (__local_main__6),Y
   LDA   HARGS+6
   DEY
   STA   (__local_main__6),Y
   LDA   __local_main__x+1
   BNE   .jcmp_skip@5
   LDA   __local_main__x
   CMP   #$02
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JMP   sdivmod16
