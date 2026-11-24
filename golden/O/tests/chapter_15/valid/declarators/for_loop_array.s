SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__counter_1	EQU	$86
__local_main__counter_0	EQU	$87
__local_main__i	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__counter_1,__local_main__counter_0,__local_main__i,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__i
   LDA   #$00
   STA   __local_main__i+1
   LDY   #$02
   STY   __local_main__i+2
   STA   __local_main__i+3
   INY
   STY   __local_main__i+4
   STA   __local_main__i+5
   STA   __local_main__counter_0
   LDY   #$00
   STY   __local_main__counter_1
.loop@0_start:
   LDA   __local_main__counter_0
   SEC
   SBC   #$03
   LDA   __local_main__counter_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__counter_0
   ASL
   STA   __local_main__0
   LDA   #<__local_main__i
   CLC
   ADC   __local_main__0
   STA   __local_main__2
   LDA   #>__local_main__i
   ADC   #$00
   STA   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   INC   __local_main__counter_0
   BNE   .inc_done@0
   INC   __local_main__counter_1
.inc_done@0:
   CMP   __local_main__counter_1
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   __local_main__counter_0
   BEQ   .loop@0_start
.jcmp_skip@1:
   LDX   #$00
   LDA   #$01
   RTS
.loop@0_break:
   LDX   #$00
   TXA
   RTS
