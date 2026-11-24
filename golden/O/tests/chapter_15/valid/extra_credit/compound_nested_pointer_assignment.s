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

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item nested_arr section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #>(nested_arr+80)
   CMP   #>(nested_arr+80)
   BNE   .jcmp_skip@0
   LDA   #<(nested_arr+80)
   CMP   #<(nested_arr+80)
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   nested_arr+83
   BNE   .jcmp_skip@1
   LDA   nested_arr+82
   BNE   .jcmp_skip@1
   LDA   nested_arr+81
   BNE   .jcmp_skip@1
   LDA   nested_arr+80
   CMP   #$64
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #>(nested_arr+80)
   STA   __local_main__7
   LDA   #<(nested_arr+80)
   SEC
   SBC   #$3C
   STA   __local_main__6
   BCS   .carry_inc@0
   DEC   __local_main__7
.carry_inc@0:
   LDY   #$04
   LDA   (__local_main__6),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__6),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__6),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__6),Y
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   BNE   .jcmp_skip@2
   LDA   __local_main__2
   BNE   .jcmp_skip@2
   LDA   __local_main__3
   CMP   #$02
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   nested_arr+1
   STA   __local_main__5
   LDA   nested_arr
   SEC
   SBC   #$09
   STA   __local_main__4
   BCS   .carry_inc@1
   DEC   __local_main__5
.carry_inc@1:
   ASL   __local_main__4
   ROL   __local_main__5
   ASL   __local_main__4
   ROL   __local_main__5
   LDA   __local_main__4
   ASL
   STA   __local_main__2
   LDA   __local_main__5
   ROL
   ASL   __local_main__2
   ROL
   STA   __local_main__3
   LDA   __local_main__2
   CLC
   ADC   __local_main__4
   STA   __local_main__1
   LDA   __local_main__3
   ADC   __local_main__5
   STA   __local_main__0
   LDA   __local_main__6
   CLC
   ADC   __local_main__1
   STA   __local_main__3
   LDA   __local_main__7
   ADC   __local_main__0
   STA   __local_main__2
   CMP   #>(nested_arr+40)
   BNE   .jcmp_skip@3
   LDA   __local_main__3
   CMP   #<(nested_arr+40)
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$DC
   CLC
   ADC   __local_main__3
   STA   __local_main__4
   LDA   #$FF
   ADC   __local_main__2
   STA   __local_main__5
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__4),Y
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   BNE   .jcmp_skip@4
   LDA   __local_main__2
   BNE   .jcmp_skip@4
   LDA   __local_main__3
   CMP   #$09
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

nested_arr:
   DC.L  $0000000A
   DC.L  $00000009
   DC.L  $00000008
   DS.B  8
   DC.L  $00000001
   DC.L  $00000002
   DS.B  52
   DC.L  $00000064
   DC.L  $00000063
   DC.L  $00000062
   DS.B  148
