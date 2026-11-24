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
__local_main__j_1	EQU	$8A
__local_main__j_0	EQU	$8B
__local_main__i_1	EQU	$8C
__local_main__i_0	EQU	$8D
__local_main__10	EQU	$8E
__local_main__11	EQU	$8F
__local_main__12	EQU	$90
__local_main__13	EQU	$91
__local_main__14	EQU	$92
__local_main__15	EQU	$93

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__j_1,__local_main__j_0,__local_main__i_1,__local_main__i_0,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item long_nested_arr section=.data local=false zero=false
; item dbl_nested_arr section=.data local=false zero=false
; item unsigned_index section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   unsigned_index
   SEC
   SBC   #$08
   ASL
   ASL
   STA   __local_main__4
   LDX   __local_main__4
   LDA   long_nested_arr+12,X
   STA   HARGS
   LDA   long_nested_arr+13,X
   STA   HARGS+1
   LDA   long_nested_arr+14,X
   STA   HARGS+2
   LDA   long_nested_arr+15,X
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS+4
   LDY   #$FF
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   __local_main__3
   LDA   HARGS+9
   STA   __local_main__2
   LDA   HARGS+10
   STA   __local_main__1
   LDA   HARGS+11
   STA   __local_main__0
   LDX   __local_main__4
   LDA   __local_main__3
   STA   long_nested_arr+12,X
   LDA   __local_main__2
   STA   long_nested_arr+13,X
   LDA   __local_main__1
   STA   long_nested_arr+14,X
   LDA   __local_main__0
   STA   long_nested_arr+15,X
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__2
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__3
   CMP   #$FA
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   long_nested_arr+20
   STA   __local_main__3
   LDA   long_nested_arr+21
   STA   __local_main__2
   LDA   long_nested_arr+22
   STA   __local_main__1
   LDA   long_nested_arr+23
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_main__2
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_main__3
   CMP   #$FA
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$02
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@2
   EOR   #$80
.jcmp_novf@2:
   BMI   .lb_skip@9
   JMP   .loop@0_break
.lb_skip@9:
   LDA   #$00
   STA   __local_main__j_0
   LDY   #$00
   STY   __local_main__j_1
.loop@1_start:
   LDA   __local_main__j_0
   SEC
   SBC   #$03
   LDA   __local_main__j_1
   SBC   #$00
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BMI   .lb_skip@8
   JMP   .loop@1_break
.lb_skip@8:
   LDA   __local_main__i_1
   BNE   .if_end@4
   LDA   __local_main__i_0
   CMP   #$01
   BNE   .vthread@0
   LDA   __local_main__j_1
   BNE   .if_end@4
   LDA   __local_main__j_0
   CMP   #$02
   BNE   .lb_skip@7
   JMP   .loop@1_break
.lb_skip@7:
.if_end@4:
   LDA   __local_main__i_0
.vthread@0:
   ASL
   STA   __local_main__1
   LDA   __local_main__i_1
   ROL
   STA   __local_main__2
   LDA   __local_main__1
   CLC
   ADC   __local_main__i_0
   STA   __local_main__0
   LDA   __local_main__2
   ADC   __local_main__i_1
   STA   __local_main__1
   LDA   __local_main__0
   CLC
   ADC   __local_main__j_0
   STA   __local_main__0
   LDA   __local_main__1
   ADC   __local_main__j_1
   STA   __local_main__9
   LDA   __local_main__0
   CLC
   ADC   #$01
   STA   __local_main__8
   BCC   .carry_inc@0
   INC   __local_main__9
.carry_inc@0:
   LDA   __local_main__9
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_main__7
   STA   __local_main__6
   LDA   __local_main__i_0
   ASL
   ASL
   STA   __local_main__1
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__0
   LDA   #<long_nested_arr
   CLC
   ADC   __local_main__0
   STA   __local_main__1
   LDA   #>long_nested_arr
   ADC   #$00
   STA   __local_main__5
   LDA   __local_main__j_0
   ASL
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__4
   BCC   .carry_inc@1
   INC   __local_main__5
.carry_inc@1:
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
   CMP   __local_main__6
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   __local_main__7
   BNE   .jcmp_skip@6
   LDA   __local_main__2
   CMP   __local_main__9
   BNE   .jcmp_skip@6
   LDA   __local_main__3
   CMP   __local_main__8
   BEQ   .if_end@5
.jcmp_skip@6:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@5:
   INC   __local_main__j_0
   BEQ   .lb_skip@6
   JMP   .loop@1_start
.lb_skip@6:
   INC   __local_main__j_1
   JMP   .loop@1_start
.loop@1_break:
   INC   __local_main__i_0
   BEQ   .lb_skip@5
   JMP   .loop@0_start
.lb_skip@5:
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   dbl_nested_arr+24
   STA   HARGS
   LDA   dbl_nested_arr+25
   STA   HARGS+1
   LDA   dbl_nested_arr+26
   STA   HARGS+2
   LDA   dbl_nested_arr+27
   STA   HARGS+3
   LDA   dbl_nested_arr+28
   STA   HARGS+4
   LDA   dbl_nested_arr+29
   STA   HARGS+5
   LDA   dbl_nested_arr+30
   STA   HARGS+6
   LDA   dbl_nested_arr+31
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$59
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   STA   dbl_nested_arr+24
   LDA   HARGS+17
   STA   dbl_nested_arr+25
   LDA   HARGS+18
   STA   dbl_nested_arr+26
   LDA   HARGS+19
   STA   dbl_nested_arr+27
   LDA   HARGS+20
   STA   dbl_nested_arr+28
   LDA   HARGS+21
   STA   dbl_nested_arr+29
   LDA   HARGS+22
   STA   dbl_nested_arr+30
   LDA   HARGS+23
   STA   dbl_nested_arr+31
   CMP   #$40
   BNE   .jcmp_skip@7
   LDA   HARGS+22
   CMP   #$69
   BNE   .jcmp_skip@7
   LDA   HARGS+21
   CMP   #$60
   BNE   .jcmp_skip@7
   LDA   HARGS+20
   BNE   .jcmp_skip@7
   LDA   HARGS+19
   BNE   .jcmp_skip@7
   LDA   HARGS+18
   BNE   .jcmp_skip@7
   LDA   HARGS+17
   BNE   .jcmp_skip@7
   LDA   HARGS+16
   BEQ   .if_end@6
.jcmp_skip@7:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@6:
   LDA   #$00
   STA   __local_main__15
   LDY   #$00
   STY   __local_main__14
.loop@2_start:
   LDA   __local_main__15
   SEC
   SBC   #$03
   LDA   __local_main__14
   SBC   #$00
   BVC   .jcmp_novf@8
   EOR   #$80
.jcmp_novf@8:
   BMI   .lb_skip@4
   JMP   .loop@2_break
.lb_skip@4:
   LDA   #$00
   STA   __local_main__13
   LDY   #$00
   STY   __local_main__12
.loop@3_start:
   LDA   __local_main__13
   SEC
   SBC   #$02
   LDA   __local_main__12
   SBC   #$00
   BVC   .jcmp_novf@9
   EOR   #$80
.jcmp_novf@9:
   BMI   .lb_skip@3
   JMP   .loop@3_break
.lb_skip@3:
   LDA   __local_main__14
   BNE   .if_end@9
   LDA   __local_main__15
   CMP   #$01
   BNE   .vthread@1
   LDA   __local_main__12
   BNE   .if_end@9
   LDA   __local_main__13
   CMP   #$01
   BNE   .lb_skip@2
   JMP   .loop@3_continue
.lb_skip@2:
.if_end@9:
   LDA   __local_main__15
.vthread@1:
   ASL
   STA   __local_main__2
   LDA   __local_main__14
   ROL
   STA   __local_main__3
   LDA   __local_main__13
   CLC
   ADC   __local_main__2
   STA   __local_main__1
   LDA   __local_main__12
   ADC   __local_main__3
   STA   __local_main__0
   LDA   __local_main__1
   CLC
   ADC   #$64
   STA   HARGS
   LDA   __local_main__0
   ADC   #$00
   STA   HARGS+1
   LDA   __local_main__15
   ASL
   ASL
   ASL
   ASL
   STA   __local_main__0
   LDA   #<dbl_nested_arr
   CLC
   ADC   __local_main__0
   STA   __local_main__3
   LDA   #>dbl_nested_arr
   ADC   #$00
   STA   __local_main__1
   LDA   __local_main__13
   ASL
   ASL
   ASL
   CLC
   ADC   __local_main__3
   STA   __local_main__0
   BCC   .carry_inc@2
   INC   __local_main__1
.carry_inc@2:
   LDY   #$00
   LDA   (__local_main__0),Y
   STA   __local_main__11
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__10
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__i_0
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__i_1
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__j_0
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__j_1
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__9
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__8
   JSR   i2d
   LDA   __local_main__8
   CMP   HARGS+9
   BNE   .jcmp_skip@10
   LDA   __local_main__9
   CMP   HARGS+8
   BNE   .jcmp_skip@10
   LDA   __local_main__j_1
   CMP   HARGS+7
   BNE   .jcmp_skip@10
   LDA   __local_main__j_0
   CMP   HARGS+6
   BNE   .jcmp_skip@10
   LDA   __local_main__i_1
   CMP   HARGS+5
   BNE   .jcmp_skip@10
   LDA   __local_main__i_0
   CMP   HARGS+4
   BNE   .jcmp_skip@10
   LDA   __local_main__10
   CMP   HARGS+3
   BNE   .jcmp_skip@10
   LDA   __local_main__11
   CMP   HARGS+2
   BEQ   .if_end@10
.jcmp_skip@10:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@10:
.loop@3_continue:
   INC   __local_main__13
   BEQ   .lb_skip@1
   JMP   .loop@3_start
.lb_skip@1:
   INC   __local_main__12
   JMP   .loop@3_start
.loop@3_break:
   INC   __local_main__15
   BEQ   .lb_skip@0
   JMP   .loop@2_start
.lb_skip@0:
   INC   __local_main__14
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   TXA
   RTS

long_nested_arr:
   DC.L  $00000001
   DC.L  $00000002
   DC.L  $00000003
   DC.L  $00000004
   DC.L  $00000005
   DC.L  $00000006

dbl_nested_arr:
   DC.L  $00000000
   DC.L  $40590000
   DC.L  $00000000
   DC.L  $40594000
   DC.L  $00000000
   DC.L  $40598000
   DC.L  $00000000
   DC.L  $4059C000
   DC.L  $00000000
   DC.L  $405A0000
   DC.L  $00000000
   DC.L  $405A4000

unsigned_index:
   DC.W  $000A
