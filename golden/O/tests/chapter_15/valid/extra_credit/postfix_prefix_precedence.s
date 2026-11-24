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
__local_main__i_1	EQU	$86
__local_main__i_0	EQU	$87
__local_main__arr	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__i_1,__local_main__i_0,__local_main__arr,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item idx section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__arr
   LDA   #$00
   STA   __local_main__arr+1
   LDY   #$02
   STY   __local_main__arr+2
   STA   __local_main__arr+3
   INY
   STY   __local_main__arr+4
   STA   __local_main__arr+5
   INY
   STY   __local_main__arr+6
   STA   __local_main__arr+7
   INY
   STY   __local_main__arr+8
   STA   __local_main__arr+9
   LDA   #>__local_main__arr
   STA   __local_main__i_1
   LDA   #<__local_main__arr
   CLC
   ADC   #$02
   STA   __local_main__4
   BCC   .carry_inc@0
   INC   __local_main__i_1
.carry_inc@0:
   LDA   idx
   ASL
   STA   __local_main__0
   LDA   idx+1
   ROL
   STA   __local_main__1
   LDA   __local_main__4
   CLC
   ADC   __local_main__0
   STA   __local_main__2
   LDA   __local_main__i_1
   ADC   __local_main__1
   STA   __local_main__3
   LDA   __local_main__4
   SEC
   SBC   #$02
   STA   __local_main__5
   BCS   .carry_inc@1
   DEC   __local_main__i_1
.carry_inc@1:
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__0
   INY
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INC   __local_main__0
   BNE   .inc_done@0
   INC   __local_main__1
.inc_done@0:
   LDA   __local_main__0
   LDY   #$00
   STA   (__local_main__2),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__2),Y
   CMP   #$00
   BNE   .jcmp_skip@0
   LDA   __local_main__0
   CMP   #$06
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$00
   LDA   (__local_main__5),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__5),Y
   STA   __local_main__2
   BNE   .jcmp_skip@1
   LDA   __local_main__3
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__i_1
   CMP   #>__local_main__arr
   BNE   .jcmp_skip@2
   LDA   __local_main__5
   CMP   #<__local_main__arr
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__2
   BNE   .jcmp_skip@3
   LDA   __local_main__3
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDY   #$02
   LDA   (__local_main__5),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__5),Y
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$04
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@5
   EOR   #$80
.jcmp_novf@5:
   BPL   .loop@0_break
   LDA   __local_main__i_0
   ASL
   STA   __local_main__0
   LDA   #<__local_main__arr
   CLC
   ADC   __local_main__0
   STA   __local_main__2
   LDA   #>__local_main__arr
   ADC   #$00
   STA   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   INC   __local_main__i_0
   BNE   .inc_done@1
   INC   __local_main__i_1
.inc_done@1:
   CMP   __local_main__i_1
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   __local_main__i_0
   BEQ   .loop@0_start
.jcmp_skip@6:
   LDX   #$00
   LDA   #$06
   RTS
.loop@0_break:
   LDA   __local_main__arr+8
   STA   __local_main__1
   LDA   __local_main__arr+9
   BNE   .jcmp_skip@7
   LDA   __local_main__1
   CMP   #$06
   BEQ   .if_end@6
.jcmp_skip@7:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS

idx:
   DC.W  $0003
