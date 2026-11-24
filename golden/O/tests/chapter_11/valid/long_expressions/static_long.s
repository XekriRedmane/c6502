SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item foo section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   foo
   CLC
   ADC   #$05
   STA   __local_main__7
   LDA   foo+1
   ADC   #$00
   STA   __local_main__6
   LDA   foo+2
   ADC   #$00
   STA   __local_main__5
   LDA   foo+3
   ADC   #$00
   STA   __local_main__4
   LDA   foo+4
   ADC   #$00
   STA   __local_main__3
   LDA   foo+5
   ADC   #$00
   STA   __local_main__2
   LDA   foo+6
   ADC   #$00
   STA   __local_main__1
   LDA   foo+7
   ADC   #$00
   BNE   .if_end@0
   LDA   __local_main__1
   BNE   .if_end@0
   LDA   __local_main__2
   BNE   .if_end@0
   LDA   __local_main__3
   BNE   .if_end@0
   LDA   __local_main__4
   CMP   #$7F
   BNE   .if_end@0
   LDA   __local_main__5
   CMP   #$FF
   BNE   .if_end@0
   LDA   __local_main__6
   CMP   #$FF
   BNE   .if_end@0
   LDA   __local_main__7
   CMP   #$FD
   BNE   .if_end@0
   LDA   #$40
   STA   foo+3
   LDA   #$00
   STA   foo
   STA   foo+1
   STA   foo+2
   STA   foo+4
   STA   foo+5
   STA   foo+6
   STA   foo+7
   BNE   .if_end@1
   LDA   foo+3
   CMP   #$40
   BNE   .if_end@1
   LDA   foo+2
   BNE   .if_end@1
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
.if_end@0:
   LDX   #$00
   TXA
   RTS

foo:
   DC.L  $7FFFFFF8
   DC.L  $00000000
