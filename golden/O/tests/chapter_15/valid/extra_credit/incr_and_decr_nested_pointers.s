SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item arr section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #<(arr+48)
   SEC
   SBC   #$30
   STA   __local_main__4
   LDA   #>(arr+48)
   SBC   #$00
   STA   __local_main__5
   LDA   #>(arr+48)
   CMP   #>(arr+48)
   BNE   .jcmp_skip@0
   LDA   #<(arr+48)
   CMP   #<(arr+48)
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$10
   CLC
   ADC   __local_main__4
   STA   __local_main__3
   LDA   #$00
   ADC   __local_main__5
   CMP   #>(arr+16)
   BNE   .jcmp_skip@1
   LDA   __local_main__3
   CMP   #<(arr+16)
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDY   #$5C
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
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   BNE   .jcmp_skip@2
   LDA   __local_main__2
   BNE   .jcmp_skip@2
   LDA   __local_main__3
   CMP   #$18
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@3:
   LDA   #>(arr+32)
   STA   __local_main__5
   LDA   arr+27
   BNE   .jcmp_skip@3
   LDA   arr+26
   BNE   .jcmp_skip@3
   LDA   arr+25
   BNE   .jcmp_skip@3
   LDA   arr+24
   CMP   #$07
   BEQ   .if_end@4
.jcmp_skip@3:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   arr+43
   BNE   .jcmp_skip@4
   LDA   arr+42
   BNE   .jcmp_skip@4
   LDA   arr+41
   BNE   .jcmp_skip@4
   LDA   arr+40
   CMP   #$0B
   BEQ   .if_end@5
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #<(arr+32)
   SEC
   SBC   #$10
   STA   __local_main__4
   BCS   .carry_inc@0
   DEC   __local_main__5
.carry_inc@0:
   LDY   #$04
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
   BNE   .jcmp_skip@5
   LDA   __local_main__1
   BNE   .jcmp_skip@5
   LDA   __local_main__2
   BNE   .jcmp_skip@5
   LDA   __local_main__3
   CMP   #$06
   BEQ   .if_end@6
.jcmp_skip@5:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   #>(arr+80)
   STA   __local_main__5
   LDA   #<(arr+80)
   SEC
   SBC   #$04
   STA   __local_main__4
   BCS   .carry_inc@1
   DEC   __local_main__5
.carry_inc@1:
   LDA   arr+91
   BNE   .jcmp_skip@6
   LDA   arr+90
   BNE   .jcmp_skip@6
   LDA   arr+89
   BNE   .jcmp_skip@6
   LDA   arr+88
   CMP   #$17
   BEQ   .if_end@7
.jcmp_skip@6:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDY   #$08
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
   BNE   .jcmp_skip@7
   LDA   __local_main__1
   BNE   .jcmp_skip@7
   LDA   __local_main__2
   BNE   .jcmp_skip@7
   LDA   __local_main__3
   CMP   #$16
   BEQ   .if_end@8
.jcmp_skip@7:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS

arr:
   DC.L  $00000001
   DC.L  $00000002
   DC.L  $00000003
   DC.L  $00000004
   DC.L  $00000005
   DC.L  $00000006
   DC.L  $00000007
   DC.L  $00000008
   DC.L  $00000009
   DC.L  $0000000A
   DC.L  $0000000B
   DC.L  $0000000C
   DC.L  $0000000D
   DC.L  $0000000E
   DC.L  $0000000F
   DC.L  $00000010
   DC.L  $00000011
   DC.L  $00000012
   DC.L  $00000013
   DC.L  $00000014
   DC.L  $00000015
   DC.L  $00000016
   DC.L  $00000017
   DC.L  $00000018
