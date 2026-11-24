SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
j	EQU	$26
i	EQU	$28
k	EQU	$2A
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__arr	EQU	$86

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__arr,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp j size=2 at=auto def=true init=$01,$00 weight=7
; zp i size=2 at=auto def=true init=$02,$00 weight=6
; zp k size=2 at=auto def=true init=$00,$00 weight=6
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
   INY
   STY   __local_main__arr+10
   STA   __local_main__arr+11
   INY
   STY   __local_main__arr+12
   STA   __local_main__arr+13
   INY
   STY   __local_main__arr+14
   STA   __local_main__arr+15
   INY
   STY   __local_main__arr+16
   STA   __local_main__arr+17
   INY
   STY   __local_main__arr+18
   STA   __local_main__arr+19
   INY
   STY   __local_main__arr+20
   STA   __local_main__arr+21
   INY
   STY   __local_main__arr+22
   STA   __local_main__arr+23
   LDA   #>__local_main__arr
   STA   __local_main__5
   JSR   main@run0
   STA   __local_main__0
   LDA   #<__local_main__arr
   CLC
   ADC   __local_main__0
   STA   __local_main__1
   BCC   .carry_inc@0
   INC   __local_main__5
.carry_inc@0:
   JSR   main@run1
   ADC   __local_main__1
   STA   __local_main__1
   BCC   .carry_inc@1
   INC   __local_main__5
.carry_inc@1:
   LDA   k
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__4
   BCC   .carry_inc@2
   INC   __local_main__5
.carry_inc@2:
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__2
   LDA   __local_main__3
   CLC
   ADC   #$01
   STA   __local_main__1
   LDA   __local_main__2
   ADC   #$00
   STA   (__local_main__4),Y
   LDA   __local_main__1
   DEY
   STA   (__local_main__4),Y
   LDA   __local_main__2
   BNE   .jcmp_skip@0
   LDA   __local_main__3
   CMP   #$0B
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>__local_main__arr
   STA   __local_main__3
   JSR   main@run0
   STA   __local_main__0
   LDA   #<__local_main__arr
   CLC
   ADC   __local_main__0
   STA   __local_main__1
   BCC   .carry_inc@3
   INC   __local_main__3
.carry_inc@3:
   JSR   main@run1
   ADC   __local_main__1
   STA   __local_main__1
   BCC   .carry_inc@4
   INC   __local_main__3
.carry_inc@4:
   LDA   k
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__2
   BCC   .carry_inc@5
   INC   __local_main__3
.carry_inc@5:
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$0C
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #>__local_main__arr
   STA   __local_main__4
   LDA   i
   SEC
   SBC   #$01
   BCS   .carry_inc@17
   DEC   i+1
.carry_inc@17:
   STA   i
   ASL
   ASL
   ASL
   STA   __local_main__0
   LDA   #<__local_main__arr
   CLC
   ADC   __local_main__0
   STA   __local_main__1
   BCC   .carry_inc@6
   INC   __local_main__4
.carry_inc@6:
   LDA   j
   STA   __local_main__0
   BNE   .carry_inc@7
   DEC   j+1
.carry_inc@7:
   DEC   j
   ASL   __local_main__0
   ASL   __local_main__0
   LDA   __local_main__1
   CLC
   ADC   __local_main__0
   STA   __local_main__2
   BCC   .carry_inc@8
   INC   __local_main__4
.carry_inc@8:
   LDA   k
   CLC
   ADC   #$01
   BCC   .carry_inc@18
   INC   k+1
.carry_inc@18:
   STA   k
   ASL
   CLC
   ADC   __local_main__2
   STA   __local_main__3
   BCC   .carry_inc@9
   INC   __local_main__4
.carry_inc@9:
   LDY   #$00
   LDA   (__local_main__3),Y
   STA   __local_main__0
   INY
   LDA   (__local_main__3),Y
   STA   __local_main__1
   INC   __local_main__0
   BNE   .inc_done@0
   INC   __local_main__1
.inc_done@0:
   LDA   __local_main__0
   LDY   #$00
   STA   (__local_main__3),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__3),Y
   CMP   #$00
   BNE   .jcmp_skip@2
   LDA   __local_main__0
   CMP   #$09
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #>__local_main__arr
   STA   __local_main__3
   JSR   main@run0
   STA   __local_main__0
   LDA   #<__local_main__arr
   CLC
   ADC   __local_main__0
   STA   __local_main__1
   BCC   .carry_inc@10
   INC   __local_main__3
.carry_inc@10:
   JSR   main@run1
   ADC   __local_main__1
   STA   __local_main__1
   BCC   .carry_inc@11
   INC   __local_main__3
.carry_inc@11:
   LDA   k
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__2
   BCC   .carry_inc@12
   INC   __local_main__3
.carry_inc@12:
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$06
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #>__local_main__arr
   STA   __local_main__3
   JSR   main@run0
   STA   __local_main__0
   LDA   #<__local_main__arr
   CLC
   ADC   __local_main__0
   STA   __local_main__1
   BCC   .carry_inc@13
   INC   __local_main__3
.carry_inc@13:
   JSR   main@run1
   ADC   __local_main__1
   STA   __local_main__1
   BCC   .carry_inc@14
   INC   __local_main__3
.carry_inc@14:
   LDA   k
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__2
   BCC   .carry_inc@15
   INC   __local_main__3
.carry_inc@15:
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__0
   INY
   LDA   (__local_main__2),Y
   STA   __local_main__1
   LDA   __local_main__0
   BNE   .carry_inc@16
   DEC   __local_main__1
.carry_inc@16:
   DEC   __local_main__0
   LDA   __local_main__0
   LDY   #$00
   STA   (__local_main__2),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__2),Y
   CMP   #$00
   BNE   .jcmp_skip@4
   LDA   __local_main__0
   CMP   #$05
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   LDA   j
   ASL
   ASL
   CLC
   RTS

main@run0:
   SUBROUTINE

   LDA   i
   ASL
   ASL
   ASL
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   i+1
   STA   j+1
   STA   k
   STA   k+1
   LDA   #$02
   STA   i
   LDA   #$01
   STA   j
   RTS
