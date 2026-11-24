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
__local_main__i	EQU	$8C
__local_main__si	EQU	$8E
__local_main__o	EQU	$99
__local_main__i2	EQU	$D3

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__i,__local_main__13,__local_main__si,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__o,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39,__local_main__40,__local_main__41,__local_main__42,__local_main__43,__local_main__44,__local_main__45,__local_main__46,__local_main__47,__local_main__48,__local_main__49,__local_main__50,__local_main__51,__local_main__52,__local_main__53,__local_main__54,__local_main__55,__local_main__56,__local_main__57,__local_main__58,__local_main__59,__local_main__60,__local_main__61,__local_main__62,__local_main__63,__local_main__64,__local_main__65,__local_main__66,__local_main__67,__local_main__68,__local_main__69,__local_main__70,__local_main__71,__local_main__72,__local_main__73,__local_main__74,__local_main__75,__local_main__76,__local_main__77,__local_main__78,__local_main__79,__local_main__80,__local_main__81,__local_main__82,__local_main__i2,__local_main__84,__local_main__85,__local_main__86 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=32
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__i
   STA   __local_main__i+1
   STA   __local_main__i2+1
   LDA   #$FE
   STA   __local_main__i2
   LDA   #$00
   STA   __local_main__si
   LDY   #$00
   STY   __local_main__si+1
   STY   __local_main__si+2
   STY   __local_main__si+3
   STA   __local_main__si+4
   LDA   #$C0
   STA   __local_main__si+5
   LDA   #$62
   STA   __local_main__si+6
   LDA   #$40
   STA   __local_main__si+7
   LDA   #$F4
   STA   __local_main__si+8
   LDA   #<__local_main__i
   STA   __local_main__si+9
   LDA   #>__local_main__i
   STA   __local_main__si+10
   LDA   #$FF
   STA   __local_main__o
   DEY
   STY   __local_main__o+1
   STY   __local_main__o+2
   STA   __local_main__o+3
   LDA   #$00
   STA   __local_main__o+4
   INY
   STY   __local_main__o+5
   STY   __local_main__o+6
   STA   __local_main__o+7
   LDA   #<__local_main__si
   STA   __local_main__o+8
   LDA   #>__local_main__si
   STA   __local_main__o+9
   TYA
   STA   __local_main__o+10
   STY   __local_main__o+11
   STY   __local_main__o+12
   STY   __local_main__o+13
   STA   __local_main__o+14
   LDA   #$C0
   STA   __local_main__o+15
   LDA   #$62
   STA   __local_main__o+16
   LDA   #$40
   STA   __local_main__o+17
   LDA   #$F4
   STA   __local_main__o+18
   LDA   #<__local_main__i
   STA   __local_main__o+19
   LDA   #>__local_main__i
   STA   __local_main__o+20
   LDA   #$50
   STA   __local_main__o+21
   LDA   #$EF
   STA   __local_main__o+22
   LDA   #$E2
   STA   __local_main__o+23
   LDA   #$D6
   STA   __local_main__o+24
   LDA   #$E4
   STA   __local_main__o+25
   LDA   #$1A
   STA   __local_main__o+26
   LDA   #$5B
   STA   __local_main__o+27
   LDA   #$C4
   STA   __local_main__o+28
   LDA   #$78
   STA   __local_main__o+29
   LDA   #$0D
   STA   __local_main__4
.clear@0:
   DEC   __local_main__4
   LDA   __local_main__4
   CLC
   ADC   #<__local_main__o
   STA   __local_main__0
   LDA   #$00
   ADC   #>__local_main__o
   STA   __local_main__1
   LDA   #$00
   LDY   #$1E
   STA   (__local_main__0),Y
   LDA   __local_main__4
   BNE   .clear@0
   LDA   #$00
   STA   __local_main__o+43
   LDY   #$00
   STY   __local_main__o+44
   STY   __local_main__o+45
   STY   __local_main__o+46
   STY   __local_main__o+47
   STA   __local_main__o+48
   LDA   #$F0
   STA   __local_main__o+49
   LDA   #$3F
   STA   __local_main__o+50
   LDA   #$01
   STA   __local_main__o+51
   LDA   #<__local_main__i2
   STA   __local_main__o+52
   LDA   #>__local_main__i2
   STA   __local_main__o+53
   LDA   #$D0
   STA   __local_main__o+54
   LDA   #$07
   STA   __local_main__o+55
   TYA
   STA   __local_main__o+56
   STA   __local_main__o+57
   LDA   __local_main__si
   STA   HARGS
   LDA   __local_main__si+1
   STA   HARGS+1
   LDA   __local_main__si+2
   STA   HARGS+2
   LDA   __local_main__si+3
   STA   HARGS+3
   LDA   __local_main__si+4
   STA   HARGS+4
   LDA   __local_main__si+5
   STA   HARGS+5
   LDA   __local_main__si+6
   STA   HARGS+6
   LDA   __local_main__si+7
   STA   HARGS+7
   TYA
   STA   HARGS+8
   JSR   main@run1
   LDA   #$24
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   STA   __local_main__si
   LDA   HARGS+17
   STA   __local_main__si+1
   LDA   HARGS+18
   STA   __local_main__si+2
   LDA   HARGS+19
   STA   __local_main__si+3
   LDA   HARGS+20
   STA   __local_main__si+4
   LDA   HARGS+21
   STA   __local_main__si+5
   LDA   HARGS+22
   STA   __local_main__si+6
   LDA   HARGS+23
   STA   __local_main__si+7
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$64
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   __local_main__o+18
   SEC
   SBC   #$CC
   STA   __local_main__o+18
   CMP   #$28
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDA   __local_main__o+21
   STA   HARGS
   LDA   __local_main__o+22
   STA   HARGS+1
   LDA   __local_main__o+23
   STA   HARGS+2
   LDA   __local_main__o+24
   STA   HARGS+3
   LDA   __local_main__o+25
   STA   HARGS+4
   LDA   __local_main__o+26
   STA   HARGS+5
   LDA   __local_main__o+27
   STA   HARGS+6
   LDA   __local_main__o+28
   JSR   main@run0
   LDA   #$10
   STA   HARGS+14
   LDA   #$C0
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+16
   STA   __local_main__o+21
   LDA   HARGS+17
   STA   __local_main__o+22
   LDA   HARGS+18
   STA   __local_main__o+23
   LDA   HARGS+19
   STA   __local_main__o+24
   LDA   HARGS+20
   STA   __local_main__o+25
   LDA   HARGS+21
   STA   __local_main__o+26
   LDA   HARGS+22
   STA   __local_main__o+27
   LDA   HARGS+23
   STA   __local_main__o+28
   CMP   #$44
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   CMP   #$7B
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   CMP   #$1A
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   CMP   #$E4
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   CMP   #$D6
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   CMP   #$E2
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   CMP   #$EF
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   CMP   #$50
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__o+8
   STA   __local_main__8
   LDA   __local_main__o+9
   STA   __local_main__9
   LDY   #$00
   LDA   (__local_main__8),Y
   STA   HARGS
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+1
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+2
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+3
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+4
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+5
   INY
   LDA   (__local_main__8),Y
   STA   HARGS+6
   INY
   LDA   (__local_main__8),Y
   JSR   main@run0
   LDA   #$14
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+17
   STA   __local_main__6
   LDA   HARGS+18
   STA   __local_main__5
   LDA   HARGS+19
   STA   __local_main__4
   LDA   HARGS+20
   STA   __local_main__3
   LDA   HARGS+21
   STA   __local_main__2
   LDA   HARGS+22
   STA   __local_main__1
   LDA   HARGS+23
   STA   __local_main__0
   LDA   HARGS+16
   LDY   #$00
   STA   (__local_main__8),Y
   LDA   __local_main__6
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__5
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__4
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__3
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__2
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__0
   INY
   STA   (__local_main__8),Y
   LDA   __local_main__si
   STA   __local_main__7
   LDA   __local_main__si+1
   STA   __local_main__6
   LDA   __local_main__si+2
   STA   __local_main__5
   LDA   __local_main__si+3
   STA   __local_main__4
   LDA   __local_main__si+4
   STA   __local_main__3
   LDA   __local_main__si+5
   STA   __local_main__2
   LDA   __local_main__si+6
   STA   __local_main__1
   LDA   __local_main__si+7
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   __local_main__2
   BNE   .jcmp_skip@2
   LDA   __local_main__3
   BNE   .jcmp_skip@2
   LDA   __local_main__4
   BNE   .jcmp_skip@2
   LDA   __local_main__5
   BNE   .jcmp_skip@2
   LDA   __local_main__6
   BNE   .jcmp_skip@2
   LDA   __local_main__7
   BEQ   .if_end@4
.jcmp_skip@2:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __local_main__o
   STA   __local_main__11
   LDA   __local_main__o+1
   STA   __local_main__10
   LDA   __local_main__o+2
   STA   __local_main__9
   LDA   __local_main__o+3
   STA   __local_main__8
   LDA   __local_main__o+4
   STA   __local_main__7
   LDA   __local_main__o+5
   STA   __local_main__6
   LDA   __local_main__o+6
   STA   __local_main__5
   LDA   __local_main__o+7
   STA   __local_main__4
   LDA   __local_main__o+54
   STA   __local_main__3
   LDA   __local_main__o+55
   STA   __local_main__2
   LDA   __local_main__o+56
   STA   __local_main__1
   LDA   __local_main__o+57
   STA   __local_main__0
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   HARGS+12
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   LDA   __local_main__11
   STA   HARGS
   LDA   __local_main__10
   STA   HARGS+1
   LDA   __local_main__9
   STA   HARGS+2
   LDA   __local_main__8
   STA   HARGS+3
   LDA   __local_main__7
   STA   HARGS+4
   LDA   __local_main__6
   STA   HARGS+5
   LDA   __local_main__5
   STA   HARGS+6
   LDA   __local_main__4
   STA   HARGS+7
   LDA   __local_main__3
   STA   HARGS+8
   LDA   __local_main__2
   STA   HARGS+9
   LDA   __local_main__1
   STA   HARGS+10
   LDA   __local_main__0
   STA   HARGS+11
   JSR   udivmod64
   LDA   HARGS+24
   STA   __local_main__o
   LDA   HARGS+25
   STA   __local_main__o+1
   LDA   HARGS+26
   STA   __local_main__o+2
   LDA   HARGS+27
   STA   __local_main__o+3
   LDA   HARGS+28
   STA   __local_main__o+4
   LDA   HARGS+29
   STA   __local_main__o+5
   LDA   HARGS+30
   STA   __local_main__o+6
   LDA   HARGS+31
   STA   __local_main__o+7
   BNE   .jcmp_skip@5
   LDA   HARGS+30
   BNE   .jcmp_skip@5
   LDA   HARGS+29
   BNE   .jcmp_skip@5
   LDA   HARGS+28
   BNE   .jcmp_skip@5
   LDA   HARGS+27
   BNE   .jcmp_skip@5
   LDA   HARGS+26
   BNE   .jcmp_skip@5
   LDA   HARGS+25
   CMP   #$05
   BNE   .jcmp_skip@5
   LDA   HARGS+24
   CMP   #$0F
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #>__local_main__o
   STA   __local_main__2
   LDA   #<__local_main__o
   CLC
   ADC   #$0A
   STA   __local_main__0
   BCC   .carry_inc@0
   INC   __local_main__2
.carry_inc@0:
   LDA   __local_main__2
   STA   __local_main__9
   LDA   __local_main__0
   CLC
   ADC   #$21
   STA   __local_main__8
   BCC   .carry_inc@1
   INC   __local_main__9
.carry_inc@1:
   STA   __local_main__o+8
   LDA   __local_main__9
   STA   __local_main__o+9
   LDY   #$00
   LDA   (__local_main__8),Y
   STA   __local_main__7
   INY
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
   LDA   __local_main__o+8
   STA   __local_main__4
   LDA   __local_main__o+9
   STA   __local_main__5
   LDY   #$09
   LDA   (__local_main__4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   CMP   #$FF
   BNE   .jcmp_skip@7
   LDA   __local_main__1
   CMP   #$FE
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   __local_main__4
   SEC
   SBC   #$0B
   BCS   .carry_inc@2
   DEC   __local_main__5
.carry_inc@2:
   STA   __local_main__o+8
   STA   __local_main__8
   LDA   __local_main__5
   STA   __local_main__o+9
   STA   __local_main__9
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
   BNE   .main@jthread@1
   LDY   #$08
   LDA   (__local_main__8),Y
   BNE   .main@jthread@1
   LDY   #$0A
   LDA   (__local_main__8),Y
   DEY
   ORA   (__local_main__8),Y
   BEQ   .if_end@12
.main@jthread@1:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@12:
   LDA   __local_main__si
   STA   __local_main__7
   LDA   __local_main__si+1
   STA   __local_main__6
   LDA   __local_main__si+2
   STA   __local_main__5
   LDA   __local_main__si+3
   STA   __local_main__4
   LDA   __local_main__si+4
   STA   __local_main__3
   LDA   __local_main__si+5
   STA   __local_main__2
   LDA   __local_main__si+6
   STA   __local_main__1
   LDA   __local_main__si+7
   CMP   #$40
   BNE   .main@jthread@3
   LDA   __local_main__1
   CMP   #$40
   BNE   .main@jthread@3
   LDA   __local_main__2
   BNE   .main@jthread@3
   LDA   __local_main__3
   BNE   .main@jthread@3
   LDA   __local_main__4
   BNE   .main@jthread@3
   LDA   __local_main__5
   BNE   .main@jthread@3
   LDA   __local_main__6
   BNE   .main@jthread@3
   LDA   __local_main__7
   BNE   .main@jthread@3
   LDA   __local_main__si+8
   STA   __local_main__0
   LDA   #$00
   CMP   #$FF
   BNE   .main@jthread@3
   LDA   __local_main__0
   CMP   #$F4
   BNE   .main@jthread@3
   LDA   __local_main__si+9
   STA   __local_main__3
   LDA   __local_main__si+10
   CMP   #>__local_main__i
   BNE   .main@jthread@3
   LDA   __local_main__3
   CMP   #<__local_main__i
   BEQ   .if_end@17
.main@jthread@3:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@17:
   LDA   HARGS+31
   BNE   .jcmp_skip@8
   LDA   HARGS+30
   BNE   .jcmp_skip@8
   LDA   HARGS+29
   BNE   .jcmp_skip@8
   LDA   HARGS+28
   BNE   .jcmp_skip@8
   LDA   HARGS+27
   BNE   .jcmp_skip@8
   LDA   HARGS+26
   BNE   .jcmp_skip@8
   LDA   HARGS+25
   CMP   #$05
   BNE   .jcmp_skip@8
   LDA   HARGS+24
   CMP   #$0F
   BEQ   .if_end@18
.jcmp_skip@8:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@18:
   LDA   __local_main__9
   CMP   #>(__local_main__o+32)
   BNE   .jcmp_skip@9
   LDA   __local_main__8
   CMP   #<(__local_main__o+32)
   BEQ   .if_end@19
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@19:
   LDA   __local_main__o+10
   STA   __local_main__7
   LDA   __local_main__o+11
   STA   __local_main__6
   LDA   __local_main__o+12
   STA   __local_main__5
   LDA   __local_main__o+13
   STA   __local_main__4
   LDA   __local_main__o+14
   STA   __local_main__3
   LDA   __local_main__o+15
   STA   __local_main__2
   LDA   __local_main__o+16
   STA   __local_main__1
   LDA   __local_main__o+17
   CMP   #$40
   BNE   .main@jthread@5
   LDA   __local_main__1
   CMP   #$62
   BNE   .main@jthread@5
   LDA   __local_main__2
   CMP   #$C0
   BNE   .main@jthread@5
   LDA   __local_main__3
   BNE   .main@jthread@5
   LDA   __local_main__4
   BNE   .main@jthread@5
   LDA   __local_main__5
   BNE   .main@jthread@5
   LDA   __local_main__6
   BNE   .main@jthread@5
   LDA   __local_main__7
   BNE   .main@jthread@5
   LDA   __local_main__o+18
   CMP   #$28
   BNE   .main@jthread@5
   LDA   __local_main__o+19
   STA   __local_main__3
   LDA   __local_main__o+20
   CMP   #>__local_main__i
   BNE   .main@jthread@5
   LDA   __local_main__3
   CMP   #<__local_main__i
   BEQ   .if_end@24
.main@jthread@5:
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@24:
   LDA   __local_main__o+21
   STA   __local_main__7
   LDA   __local_main__o+22
   STA   __local_main__6
   LDA   __local_main__o+23
   STA   __local_main__5
   LDA   __local_main__o+24
   STA   __local_main__4
   LDA   __local_main__o+25
   STA   __local_main__3
   LDA   __local_main__o+26
   STA   __local_main__2
   LDA   __local_main__o+27
   STA   __local_main__1
   LDA   __local_main__o+28
   CMP   #$44
   BNE   .main@jthread@7
   LDA   __local_main__1
   CMP   #$7B
   BNE   .main@jthread@7
   LDA   __local_main__2
   CMP   #$1A
   BNE   .main@jthread@7
   LDA   __local_main__3
   CMP   #$E4
   BNE   .main@jthread@7
   LDA   __local_main__4
   CMP   #$D6
   BNE   .main@jthread@7
   LDA   __local_main__5
   CMP   #$E2
   BNE   .main@jthread@7
   LDA   __local_main__6
   CMP   #$EF
   BNE   .main@jthread@7
   LDA   __local_main__7
   CMP   #$50
   BNE   .main@jthread@7
   LDA   __local_main__o+29
   CMP   #$78
   BNE   .main@jthread@7
   LDA   __local_main__o+30
   ORA   __local_main__o+31
   BEQ   .if_end@29
.main@jthread@7:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@29:
   LDA   __local_main__o+32
   LDX   __local_main__o+33
   STX   __local_main__6
   LDX   __local_main__o+34
   STX   __local_main__5
   LDX   __local_main__o+35
   STX   __local_main__4
   LDX   __local_main__o+36
   STX   __local_main__3
   LDX   __local_main__o+37
   STX   __local_main__2
   LDX   __local_main__o+38
   STX   __local_main__1
   LDX   __local_main__o+39
   STX   __local_main__0
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .main@jthread@9
   LDA   __local_main__o+40
   BNE   .main@jthread@9
   LDA   __local_main__o+41
   ORA   __local_main__o+42
   BEQ   .if_end@34
.main@jthread@9:
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@34:
   LDA   __local_main__o+43
   STA   __local_main__7
   LDA   __local_main__o+44
   STA   __local_main__6
   LDA   __local_main__o+45
   STA   __local_main__5
   LDA   __local_main__o+46
   STA   __local_main__4
   LDA   __local_main__o+47
   STA   __local_main__3
   LDA   __local_main__o+48
   STA   __local_main__2
   LDA   __local_main__o+49
   STA   __local_main__1
   LDA   __local_main__o+50
   CMP   #$3F
   BNE   .main@jthread@11
   LDA   __local_main__1
   CMP   #$F0
   BNE   .main@jthread@11
   LDA   __local_main__2
   BNE   .main@jthread@11
   LDA   __local_main__3
   BNE   .main@jthread@11
   LDA   __local_main__4
   BNE   .main@jthread@11
   LDA   __local_main__5
   BNE   .main@jthread@11
   LDA   __local_main__6
   BNE   .main@jthread@11
   LDA   __local_main__7
   BNE   .main@jthread@11
   LDA   __local_main__o+51
   CMP   #$01
   BNE   .main@jthread@11
   LDA   __local_main__o+52
   STA   __local_main__3
   LDA   __local_main__o+53
   CMP   #>__local_main__i2
   BNE   .main@jthread@11
   LDA   __local_main__3
   CMP   #<__local_main__i2
   BEQ   .if_end@39
.main@jthread@11:
   LDX   #$00
   LDA   #$10
   RTS
.if_end@39:
   LDA   __local_main__o+54
   STA   __local_main__3
   LDA   __local_main__o+55
   STA   __local_main__2
   LDA   __local_main__o+56
   STA   __local_main__1
   LDA   __local_main__o+57
   BNE   .jcmp_skip@10
   LDA   __local_main__1
   BNE   .jcmp_skip@10
   LDA   __local_main__2
   CMP   #$07
   BNE   .jcmp_skip@10
   LDA   __local_main__3
   CMP   #$D0
   BEQ   .if_end@40
.jcmp_skip@10:
   LDX   #$00
   LDA   #$11
   RTS
.if_end@40:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   JSR   main@run1
   RTS
