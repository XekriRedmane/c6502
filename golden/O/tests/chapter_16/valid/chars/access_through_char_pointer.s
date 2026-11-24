SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__array_17	EQU	$80
__local_main__array_19	EQU	$81
__local_main__array_18	EQU	$82
__local_main__array_16	EQU	$83
__local_main__x	EQU	$86
__local_main__d	EQU	$8A

; @zp-link-meta-begin
; def main params= locals=__local_main__array_17,__local_main__array_19,__local_main__array_18,__local_main__array_16,__local_main__0,__local_main__1,__local_main__x,__local_main__3,__local_main__4,__local_main__5,__local_main__d,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$64
   STA   __local_main__x
   LDA   #$00
   STA   __local_main__x+1
   STA   __local_main__x+2
   STA   __local_main__x+3
   LDY   #$00
   LDA   __local_main__x,Y
   CMP   #$64
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$01
   LDA   __local_main__x,Y
   BNE   .main@jthread@1
   INY
   LDA   __local_main__x,Y
   BNE   .main@jthread@1
   INY
   LDA   __local_main__x,Y
   BEQ   .if_end@5
.main@jthread@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@5:
   LDA   #$00
   STA   __local_main__d
   STA   __local_main__d+1
   STA   __local_main__d+2
   STA   __local_main__d+3
   STA   __local_main__d+4
   STA   __local_main__d+5
   STA   __local_main__d+6
   LDA   #$80
   STA   __local_main__d+7
   LDY   #$07
   LDA   __local_main__d,Y
   BPL   .jcmp_skip@2
   CMP   #$80
   BEQ   .if_end@6
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@6:
   LDA   #$00
   STA   __local_main__array_16
   LDY   #$00
   STY   __local_main__array_18
.loop@0_start:
   LDA   __local_main__array_16
   SEC
   SBC   #$07
   LDA   __local_main__array_18
   SBC   #$00
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BPL   .vthread@1
   LDA   #<__local_main__d
   CLC
   ADC   __local_main__array_16
   STA   __local_main__array_17
   LDA   #>__local_main__d
   ADC   __local_main__array_18
   STA   __local_main__array_19
   LDY   #$00
   LDA   (__local_main__array_17),Y
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$04
   RTS
.if_end@7:
   INC   __local_main__array_16
   BNE   .loop@0_start
   INC   __local_main__array_18
   JMP   .loop@0_start
.vthread@1:
   LDA   #$FF
   CMP   #$FF
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$06
   RTS
.if_end@11:
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
.sx_neg@7:
   CMP   #$FF
   BNE   .jcmp_skip@9
   LDA   #$FF
   CMP   #$FF
   BEQ   .if_end@12
.jcmp_skip@9:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@12:
   LDX   #$00
   TXA
   RTS
