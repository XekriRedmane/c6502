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
__local_main__8	EQU	$88
__local_main__9	EQU	$89
__local_main__10	EQU	$8A
__local_main__11	EQU	$8B
__local_main__13	EQU	$8D
__local_main__x	EQU	$8E

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__x,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__x
   LDY   #$00
   STY   __local_main__x+1
   STY   __local_main__x+2
   STY   __local_main__x+3
   STY   __local_main__x+4
   STY   __local_main__x+5
   STY   __local_main__x+6
   STY   __local_main__x+7
   STY   __local_main__x+8
   STY   __local_main__x+9
   STY   __local_main__x+10
   STY   __local_main__x+11
   STY   __local_main__x+12
   STA   __local_main__x+13
   LDA   #$F0
   STA   __local_main__x+14
   LDA   #$3F
   STA   __local_main__x+15
   TYA
   STA   __local_main__x+16
   STY   __local_main__x+17
   STY   __local_main__x+18
   STY   __local_main__x+19
   STY   __local_main__x+20
   STY   __local_main__x+21
   STA   __local_main__x+22
   LDA   #$40
   STA   __local_main__x+23
   LDA   #>__local_main__x
   STA   __local_main__13
   JSR   main@run0
   STA   __local_main__9
   BCC   .carry_inc@0
   INC   __local_main__13
.carry_inc@0:
   LDA   #>__local_main__x
   STA   __local_main__2
   JSR   main@run0
   STA   __local_main__0
   BCC   .carry_inc@1
   INC   __local_main__2
.carry_inc@1:
   LDA   __local_main__13
   CMP   __local_main__2
   BNE   .jcmp_skip@0
   LDA   __local_main__9
   CMP   __local_main__0
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   TYA
   STA   __local_main__7
   STA   __local_main__2
   LDA   #$3F
   CMP   #$3F
   BNE   .jcmp_skip@1
   LDA   #$F0
   CMP   #$F0
   BNE   .jcmp_skip@1
   LDA   __local_main__2
   BNE   .jcmp_skip@1
   TYA
   BNE   .jcmp_skip@1
   LDA   __local_main__7
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__9
   CLC
   ADC   #$08
   STA   __local_main__8
   LDA   __local_main__13
   ADC   #$00
   STA   __local_main__11
   LDA   #>__local_main__x
   STA   __local_main__2
   JSR   main@run0
   STA   __local_main__0
   BCC   .carry_inc@2
   INC   __local_main__2
.carry_inc@2:
   LDA   __local_main__13
   CMP   __local_main__2
   BNE   .jcmp_skip@2
   LDA   __local_main__9
   CMP   __local_main__0
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #>__local_main__x
   STA   __local_main__2
   LDA   #<__local_main__x
   CLC
   ADC   #$10
   STA   __local_main__0
   BCC   .carry_inc@3
   INC   __local_main__2
.carry_inc@3:
   LDA   __local_main__11
   CMP   __local_main__2
   BNE   .jcmp_skip@3
   LDA   __local_main__8
   CMP   __local_main__0
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   TYA
   STA   __local_main__7
   STA   __local_main__1
   LDA   #$40
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   BNE   .jcmp_skip@4
   TYA
   BNE   .jcmp_skip@4
   LDA   __local_main__7
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __local_main__8
   SEC
   SBC   #$08
   STA   __local_main__10
   BCS   .carry_inc@4
   DEC   __local_main__11
.carry_inc@4:
   LDA   #>__local_main__x
   STA   __local_main__2
   JSR   main@run0
   STA   __local_main__0
   BCC   .carry_inc@5
   INC   __local_main__2
.carry_inc@5:
   LDA   __local_main__11
   CMP   __local_main__2
   BNE   .jcmp_skip@5
   LDA   __local_main__10
   CMP   __local_main__0
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDY   #$00
   LDA   (__local_main__10),Y
   STA   __local_main__7
   INY
   LDA   (__local_main__10),Y
   STA   __local_main__6
   INY
   LDA   (__local_main__10),Y
   STA   __local_main__5
   INY
   LDA   (__local_main__10),Y
   STA   __local_main__4
   INY
   LDA   (__local_main__10),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__10),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__10),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__10),Y
   CMP   #$3F
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   #$F0
   BNE   .jcmp_skip@6
   LDA   __local_main__2
   BNE   .jcmp_skip@6
   LDA   __local_main__3
   BNE   .jcmp_skip@6
   LDA   __local_main__4
   BNE   .jcmp_skip@6
   LDA   __local_main__5
   BNE   .jcmp_skip@6
   LDA   __local_main__6
   BNE   .jcmp_skip@6
   LDA   __local_main__7
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   __local_main__10
   SEC
   SBC   #$08
   STA   __local_main__8
   LDA   __local_main__11
   SBC   #$00
   STA   __local_main__9
   LDA   #>__local_main__x
   STA   __local_main__2
   JSR   main@run0
   STA   __local_main__0
   BCC   .carry_inc@6
   INC   __local_main__2
.carry_inc@6:
   LDA   __local_main__11
   CMP   __local_main__2
   BNE   .jcmp_skip@7
   LDA   __local_main__10
   CMP   __local_main__0
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDY   #$01
   LDA   (__local_main__8),Y
   STA   __local_main__6
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__5
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__4
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__0
   LDY   #$00
   LDA   (__local_main__8),Y
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   __local_main__9
   CMP   #>__local_main__x
   BNE   .jcmp_skip@8
   LDA   __local_main__8
   CMP   #<__local_main__x
   BEQ   .if_end@9
.jcmp_skip@8:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   LDA   #<__local_main__x
   CLC
   ADC   #$08
   RTS
