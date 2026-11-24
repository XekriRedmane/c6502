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
__local_main__arr	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__arr,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__arr
   STA   __local_main__arr+1
   LDY   #$FE
   STY   __local_main__arr+2
   STA   __local_main__arr+3
   LDY   #$FC
   STY   __local_main__arr+6
   STA   __local_main__arr+7
   LDA   #>__local_main__arr
   STA   __local_main__6
   LDA   #<__local_main__arr
   CLC
   ADC   #$02
   STA   __local_main__4
   BCC   .carry_inc@0
   INC   __local_main__6
.carry_inc@0:
   LDA   #$FD
   LDY   #$FF
   ASL
   STA   __local_main__2
   TYA
   ROL
   STA   __local_main__3
   LDA   __local_main__2
   CLC
   ADC   #$FD
   STA   __local_main__1
   TYA
   ADC   __local_main__3
   STA   __local_main__arr+5
   LDX   __local_main__1
   STX   __local_main__arr+4
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$F7
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$FE
   CPY   #$FF
   BNE   .jcmp_skip@1
   CMP   #$FE
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__4
   SEC
   SBC   #$02
   STA   __local_main__5
   BCS   .carry_inc@1
   DEC   __local_main__6
.carry_inc@1:
   CLC
   STA   DPTR
   LDA   __local_main__6
   STA   DPTR+1
   LDY   #$07
   LDA   (__local_main__5),Y
   STA   __local_main__1
   DEY
   LDA   (__local_main__5),Y
   ADC   #$04
   STA   __local_main__0
   BCC   .carry_inc@2
   INC   __local_main__1
.carry_inc@2:
   LDY   #$06
   STA   (__local_main__5),Y
   LDA   __local_main__1
   INY
   STA   (DPTR),Y
   LDA   __local_main__0
   ORA   __local_main__1
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__arr
   STA   __local_main__1
   LDA   __local_main__arr+1
   CMP   #$FF
   BNE   .main@jthread@2
   LDA   __local_main__1
   CMP   #$FF
   BNE   .main@jthread@2
   LDA   __local_main__arr+2
   STA   __local_main__1
   LDA   __local_main__arr+3
   CMP   #$FF
   BNE   .main@jthread@2
   LDA   __local_main__1
   CMP   #$FE
   BNE   .main@jthread@2
   LDA   __local_main__arr+4
   STA   __local_main__1
   LDA   __local_main__arr+5
   CMP   #$FF
   BNE   .main@jthread@2
   LDA   __local_main__1
   CMP   #$F7
   BNE   .main@jthread@2
   LDA   __local_main__arr+6
   ORA   __local_main__arr+7
   BEQ   .if_end@10
.main@jthread@2:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@10:
   LDX   #$00
   TXA
   RTS
