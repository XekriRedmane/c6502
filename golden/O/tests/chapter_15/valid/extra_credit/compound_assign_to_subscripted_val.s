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
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item unsigned_arr section=.data local=false zero=false
; item idx section=.data local=false zero=false
; item long_idx section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   SEC
   LDA   #$00
   SBC   long_idx
   STA   long_idx
   LDA   #$00
   SBC   long_idx+1
   STA   long_idx+1
   LDA   #$00
   SBC   long_idx+2
   STA   long_idx+2
   LDA   #$00
   SBC   long_idx+3
   STA   long_idx+3
   LDA   unsigned_arr+4
   CLC
   ADC   #$02
   STA   unsigned_arr+4
   BCC   .carry_inc@0
   INC   unsigned_arr+5
   BNE   .carry_inc@0
   INC   unsigned_arr+6
   BNE   .carry_inc@0
   INC   unsigned_arr+7
.carry_inc@0:
   LDX   unsigned_arr+5
   STX   __local_main__2
   LDX   unsigned_arr+6
   STX   __local_main__1
   LDX   unsigned_arr+7
   STX   __local_main__0
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   idx
   ASL
   ASL
   STA   __local_main__4
   LDX   __local_main__4
   JSR   main@run0
   JSR   ul2d
   LDA   HARGS+4
   STA   HARGS
   LDA   HARGS+5
   STA   HARGS+1
   LDA   HARGS+6
   STA   HARGS+2
   LDA   HARGS+7
   STA   HARGS+3
   LDA   HARGS+8
   STA   HARGS+4
   LDA   HARGS+9
   STA   HARGS+5
   LDA   HARGS+10
   STA   HARGS+6
   LDA   HARGS+11
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$24
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dsub
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
   LDA   HARGS+9
   STA   __local_main__2
   LDA   HARGS+10
   STA   __local_main__1
   LDA   HARGS+11
   STA   __local_main__0
   LDX   __local_main__4
   LDA   HARGS+8
   STA   unsigned_arr,X
   LDA   __local_main__2
   STA   unsigned_arr+1,X
   LDA   __local_main__1
   STA   unsigned_arr+2,X
   LDA   __local_main__0
   STA   unsigned_arr+3,X
   LDA   idx
   ASL
   ASL
   TAX
   LDA   unsigned_arr,X
   STA   __local_main__3
   LDA   unsigned_arr+1,X
   STA   __local_main__2
   LDA   unsigned_arr+2,X
   STA   __local_main__1
   LDA   unsigned_arr+3,X
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   BNE   .jcmp_skip@0
   LDA   __local_main__2
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__3
   CMP   #$F3
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #<(unsigned_arr+16)
   STA   __local_main__7
   LDA   #>(unsigned_arr+16)
   STA   __local_main__6
   LDA   long_idx
   STA   __local_main__0
   LDA   long_idx+1
   ASL   __local_main__0
   ROL
   ASL   __local_main__0
   ROL
   STA   __local_main__1
   LDA   #<(unsigned_arr+16)
   CLC
   ADC   __local_main__0
   STA   __local_main__4
   LDA   #>(unsigned_arr+16)
   ADC   __local_main__1
   STA   __local_main__5
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   HARGS
   INY
   LDA   (__local_main__4),Y
   STA   HARGS+1
   INY
   LDA   (__local_main__4),Y
   STA   HARGS+2
   INY
   LDA   (__local_main__4),Y
   STA   HARGS+3
   JSR   main@run1
   LDA   HARGS+9
   STA   __local_main__2
   LDA   HARGS+10
   STA   __local_main__1
   LDA   HARGS+11
   STA   __local_main__0
   LDA   HARGS+8
   LDY   #$00
   STA   (__local_main__4),Y
   LDA   __local_main__2
   INY
   STA   (__local_main__4),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__4),Y
   LDA   __local_main__0
   INY
   STA   (__local_main__4),Y
   LDA   unsigned_arr+12
   STA   __local_main__3
   LDA   unsigned_arr+13
   STA   __local_main__2
   LDA   unsigned_arr+14
   STA   __local_main__1
   LDA   unsigned_arr+15
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   BNE   .jcmp_skip@1
   LDA   __local_main__2
   CMP   #$19
   BNE   .jcmp_skip@1
   LDA   __local_main__3
   CMP   #$99
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   long_idx
   ASL
   STA   __local_main__0
   LDA   long_idx+1
   ROL
   STA   __local_main__1
   LDA   long_idx+2
   ROL
   STA   __local_main__2
   LDA   long_idx+3
   ROL
   LDX   __local_main__0
   STX   long_idx
   LDX   __local_main__1
   STX   long_idx+1
   LDX   __local_main__2
   STX   long_idx+2
   STA   long_idx+3
   ASL   __local_main__0
   ROL   __local_main__1
   ASL   __local_main__0
   ROL   __local_main__1
   LDA   __local_main__7
   CLC
   ADC   __local_main__0
   STA   __local_main__4
   LDA   __local_main__6
   ADC   __local_main__1
   STA   __local_main__5
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   HARGS
   INY
   LDA   (__local_main__4),Y
   STA   HARGS+1
   INY
   LDA   (__local_main__4),Y
   STA   HARGS+2
   INY
   LDA   (__local_main__4),Y
   STA   HARGS+3
   LDA   unsigned_arr
   STA   HARGS+4
   LDA   unsigned_arr+1
   STA   HARGS+5
   LDA   unsigned_arr+2
   STA   HARGS+6
   LDA   unsigned_arr+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+9
   STA   __local_main__2
   LDA   HARGS+10
   STA   __local_main__1
   LDA   HARGS+11
   STA   __local_main__0
   LDA   HARGS+8
   LDY   #$00
   STA   (__local_main__4),Y
   LDA   __local_main__2
   INY
   STA   (__local_main__4),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__4),Y
   LDA   __local_main__0
   INY
   STA   (__local_main__4),Y
   LDA   unsigned_arr+8
   STA   __local_main__3
   LDA   unsigned_arr+9
   STA   __local_main__2
   LDA   unsigned_arr+10
   STA   __local_main__1
   LDA   unsigned_arr+11
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   BNE   .jcmp_skip@2
   LDA   __local_main__2
   BNE   .jcmp_skip@2
   LDA   __local_main__3
   CMP   #$0D
   BEQ   .vthread@0
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04
   RTS
.vthread@0:
   LDA   idx
   CLC
   ADC   long_idx
   ASL
   ASL
   STA   __local_main__4
   LDX   __local_main__4
   JSR   main@run0
   JSR   main@run1
   LDA   HARGS+12
   STA   __local_main__3
   LDA   HARGS+13
   STA   __local_main__2
   LDA   HARGS+14
   STA   __local_main__1
   LDA   HARGS+15
   STA   __local_main__0
   LDX   __local_main__4
   LDA   __local_main__3
   STA   unsigned_arr,X
   LDA   __local_main__2
   STA   unsigned_arr+1,X
   LDA   __local_main__1
   STA   unsigned_arr+2,X
   LDA   __local_main__0
   STA   unsigned_arr+3,X
   BNE   .jcmp_skip@5
   LDA   __local_main__1
   BNE   .jcmp_skip@5
   LDA   __local_main__2
   BNE   .jcmp_skip@5
   LDA   __local_main__3
   CMP   #$05
   BEQ   .if_end@4
.jcmp_skip@5:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   unsigned_arr
   STA   __local_main__3
   LDA   unsigned_arr+1
   STA   __local_main__2
   LDA   unsigned_arr+2
   STA   __local_main__1
   LDA   unsigned_arr+3
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   BNE   .jcmp_skip@6
   LDA   __local_main__2
   BNE   .jcmp_skip@6
   LDA   __local_main__3
   CMP   #$05
   BEQ   .if_end@5
.jcmp_skip@6:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   unsigned_arr+4
   LDX   unsigned_arr+5
   STX   __local_main__2
   LDX   unsigned_arr+6
   STX   __local_main__1
   LDX   unsigned_arr+7
   STX   __local_main__0
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   unsigned_arr+8
   STA   __local_main__3
   LDA   unsigned_arr+9
   STA   __local_main__2
   LDA   unsigned_arr+10
   STA   __local_main__1
   LDA   unsigned_arr+11
   BNE   .jcmp_skip@7
   LDA   __local_main__1
   BNE   .jcmp_skip@7
   LDA   __local_main__2
   BNE   .jcmp_skip@7
   LDA   __local_main__3
   CMP   #$0D
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   unsigned_arr+12
   STA   __local_main__3
   LDA   unsigned_arr+13
   STA   __local_main__2
   LDA   unsigned_arr+14
   STA   __local_main__1
   LDA   unsigned_arr+15
   BNE   .jcmp_skip@8
   LDA   __local_main__1
   BNE   .jcmp_skip@8
   LDA   __local_main__2
   CMP   #$19
   BNE   .jcmp_skip@8
   LDA   __local_main__3
   CMP   #$99
   BEQ   .if_end@8
.jcmp_skip@8:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   LDA   #$0A
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JMP   udivmod32

main@run0:
   SUBROUTINE

   LDA   unsigned_arr,X
   STA   HARGS
   LDA   unsigned_arr+1,X
   STA   HARGS+1
   LDA   unsigned_arr+2,X
   STA   HARGS+2
   LDA   unsigned_arr+3,X
   STA   HARGS+3
   RTS

unsigned_arr:
   DC.L  $0000FFFF
   DC.L  $0000FFFE
   DC.L  $0000FFFD
   DC.L  $0000FFFC

idx:
   DC.W  $0002

long_idx:
   DC.L  $00000001
