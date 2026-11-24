SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__zpabi_f0__p1	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f1__p0_1	EQU	$81
__zpabi_f1__p0_2	EQU	$82
__zpabi_f1__p0_3	EQU	$83
__zpabi_f1__p1_0	EQU	$84
__zpabi_f1__p1_1	EQU	$85
__zpabi_f1__p1_2	EQU	$86
__zpabi_f1__p1_3	EQU	$87
__zpabi_f1__p2_0	EQU	$88
__zpabi_f1__p2_1	EQU	$89
__zpabi_f1__p2_2	EQU	$8A
__zpabi_f1__p2_3	EQU	$8B
__zpabi_f1__pp_0	EQU	$8C
__zpabi_f1__pp_1	EQU	$8D
__zpabi_f2__p0_0	EQU	$8E
__zpabi_f2__p0_1	EQU	$8F
__zpabi_f2__p0_2	EQU	$90
__zpabi_f2__p0_3	EQU	$91
__zpabi_f2__p1_0	EQU	$92
__zpabi_f2__p1_1	EQU	$93
__zpabi_f2__p1_2	EQU	$94
__zpabi_f2__p1_3	EQU	$95
__zpabi_f2__p3_0	EQU	$96
__zpabi_f2__p3_1	EQU	$97
__zpabi_f2__p3_2	EQU	$98
__zpabi_f2__p3_3	EQU	$99
__local_main__0	EQU	$9A
__local_f1__1	EQU	$9B
__local_main__1	EQU	$9B
__local_f1__2	EQU	$9C
__local_main__2	EQU	$9C
__local_main__3	EQU	$9D
__local_main__4	EQU	$9E
__local_f1__5	EQU	$9F
__local_main__5	EQU	$9F
__local_f1__6	EQU	$A0
__local_f1__8	EQU	$A2
__local_f0__0	EQU	$A8
__local_f0__2	EQU	$AA
__local_f0__it_1	EQU	$AE
__local_f0__it_0	EQU	$AF
__local_f0__l1_3	EQU	$B0
__local_f0__l1_2	EQU	$B1
__local_f0__l1_1	EQU	$B2
__local_f0__l1_0	EQU	$B3
__local_f0__6	EQU	$B4
__local_f0__p0_1	EQU	$B5
__local_f0__p0_0	EQU	$B6
__local_f2__0	EQU	$B7
__local_f2__1	EQU	$B8

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__it_1,__local_f0__it_0,__local_f0__l1_3,__local_f0__l1_2,__local_f0__l1_1,__local_f0__l1_0,__local_f0__6,__local_f0__p0_1,__local_f0__p0_0 indirect=false in_cycle=false param_regs=A,X,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8 indirect=false in_cycle=false carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p3_0,__zpabi_f2__p3_1,__zpabi_f2__p3_2,__zpabi_f2__p3_3 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false param_regs=,,,,,,,,A,X,,,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f0 -> f2
; call f1 -> f2
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$25 weight=11
; zp g0 size=2 at=auto def=true init=$09,$00 weight=14
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f2:
   SUBROUTINE

   LDA   g0+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __zpabi_f2__p3_0
   SEC
   SBC   g0
   LDA   __zpabi_f2__p3_1
   SBC   g0+1
   LDA   __zpabi_f2__p3_2
   SBC   __local_f2__1
   LDA   __zpabi_f2__p3_3
   SBC   __local_f2__0
   LDA   #$00
   ROL
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   BEQ   .cmp_end@3
   SEC
   RTS
.cmp_end@3:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   LDA   g0+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   CMP   __zpabi_f1__p0_3
   BNE   .thread@0
   CMP   __zpabi_f1__p0_2
   BNE   .thread@0
   LDA   g0+1
   CMP   __zpabi_f1__p0_1
   BNE   .thread@0
   LDA   g0
   CMP   __zpabi_f1__p0_0
   BEQ   .cmp_true@6
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_f1__8
   AND   #$07
   STA   HARGS+4
   LDA   __zpabi_f1__p0_0
   STA   HARGS
   LDA   __zpabi_f1__p0_1
   STA   HARGS+1
   LDA   __zpabi_f1__p0_2
   STA   HARGS+2
   LDA   __zpabi_f1__p0_3
   STA   HARGS+3
   JSR   asl32
   LDA   __zpabi_f1__p0_0
   AND   #$07
   TAX
   LDA   #$00
   STA   __local_f1__2
   STA   __zpabi_f2__p1_2
   CPX   #$00
   BEQ   .shift_done@10
.shift_loop@9:
   ASL   __local_f1__2
   ROL   __zpabi_f2__p1_2
   DEX
   BNE   .shift_loop@9
.shift_done@10:
   LDA   __zpabi_f2__p1_2
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f1__1
   STA   __zpabi_f2__p3_0
   LDA   __local_f1__2
   SEC
   SBC   HARGS+5
   LDA   __zpabi_f2__p1_2
   SBC   HARGS+6
   LDA   __local_f1__1
   SBC   HARGS+7
   LDA   __zpabi_f2__p3_0
   SBC   HARGS+8
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f2__p3_0
   LDY   #$00
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__2
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   LDA   #$00
   BNE   .cmp_true@14
   LDA   __zpabi_f2__p3_0
   CMP   #$FF
   BNE   .cmp_true@14
   LDA   #$00
   BEQ   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   CLC
   ADC   __local_f1__2
   BCC   .carry_inc@0
   INC   __local_f1__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   INY
   STA   (__zpabi_f1__pp_0),Y
   LDA   __zpabi_f1__p2_0
   SEC
   SBC   __local_f1__8
   LDA   __zpabi_f1__p2_1
   SBC   #$00
   LDA   __zpabi_f1__p2_2
   SBC   #$00
   LDA   __zpabi_f1__p2_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __zpabi_f2__p1_0
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __zpabi_f2__p1_2
   LDA   __zpabi_f1__p2_0
   AND   #$07
   TAX
   LDA   __local_f1__8
   STA   __local_f1__1
   CPX   #$00
   BEQ   .shift_done@20
.shift_loop@19:
   LSR   __local_f1__1
   DEX
   BNE   .shift_loop@19
.shift_done@20:
   LDA   __zpabi_f1__p1_0
   SEC
   SBC   #$02
   LDA   __zpabi_f1__p1_1
   SBC   #$00
   LDA   __zpabi_f1__p1_2
   SBC   #$00
   LDA   __zpabi_f1__p1_3
   SBC   #$00
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f2__p3_0
   LDA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_f2__p3_2
   STA   __zpabi_f2__p3_3
   LDA   __local_f1__8
   STA   __zpabi_f2__p0_0
   LDA   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_3
   LDA   #$00
   STA   __zpabi_f2__p0_1
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   STA   __zpabi_f2__p1_1
   STA   __zpabi_f2__p3_1
   LDX   #$00
   LDA   __local_f1__1
   JSR   f2
   LDA   __zpabi_f1__p0_0
   EOR   #$FF
   STA   __local_f1__6
   LDA   __zpabi_f1__p0_1
   EOR   #$FF
   STA   __local_f1__5
   LDA   __zpabi_f1__p0_2
   EOR   #$FF
   STA   __zpabi_f2__p1_0
   LDA   __zpabi_f1__p0_3
   EOR   #$FF
   STA   __zpabi_f2__p1_2
   LDA   __zpabi_f1__p1_0
   SEC
   SBC   __zpabi_f1__p1_0
   LDA   __zpabi_f1__p1_1
   SBC   __zpabi_f1__p1_1
   LDA   __zpabi_f1__p1_2
   SBC   __zpabi_f1__p1_2
   LDA   __zpabi_f1__p1_3
   SBC   __zpabi_f1__p1_3
   BVC   .cmp_novf@24
   EOR   #$80
.cmp_novf@24:
   ASL
   LDA   #$00
   ROL
   STA   __local_f1__2
   LDA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   __local_f1__1
   STA   __zpabi_f2__p3_0
   LDA   __local_f1__2
   SEC
   SBC   __local_f1__6
   LDA   #$00
   SBC   __local_f1__5
   LDA   __local_f1__1
   SBC   __zpabi_f2__p1_0
   LDA   __zpabi_f2__p3_0
   SBC   __zpabi_f2__p1_2
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
   ASL
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   STX   __local_f0__p0_1
   CLC
   ADC   g1
   STA   __local_f0__6
   LDA   #$01
   STA   __local_f0__l1_0
   LDA   #$00
   STA   __local_f0__l1_1
   STA   __local_f0__l1_2
   STA   __local_f0__l1_3
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__it_1
.loop@0_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@28
   EOR   #$80
.jcmp_novf@28:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   __local_f0__p0_0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__p0_1
   STA   HARGS+3
   LDA   #$E8
   STA   HARGS
   LDA   #$03
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __zpabi_f2__p0_2
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@6
   LDA   #$01
   STA   __zpabi_f2__p3_0
   LDA   #$00
   BEQ   .and_end@7
.and_false@6:
   LDA   #$00
   STA   __zpabi_f2__p3_0
.and_end@7:
   ORA   #$00
   BPL   .sx_done@32
   LDA   #$FF
.sx_done@32:
   STA   __zpabi_f2__p3_2
   STA   __zpabi_f2__p3_3
   LDA   HARGS+6
   STA   __zpabi_f2__p0_0
   LDA   HARGS+7
   STA   __zpabi_f2__p0_1
   LDA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   LDA   #$01
   STA   __zpabi_f2__p1_0
   LDA   #$00
   STA   __zpabi_f2__p1_1
   STA   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_3
   STA   __zpabi_f2__p3_1
   LDX   __local_f0__p0_1
   LDA   __local_f0__p0_0
   JSR   f2
   LDA   #$00
   ROL
   CLC
   ADC   __local_f0__it_0
   STA   __zpabi_f2__p0_2
   LDA   #$00
   ADC   __local_f0__it_1
   STA   __local_f0__2
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __zpabi_f2__p3_0
   STA   __local_f0__0
   LDA   __local_f0__l1_0
   CLC
   ADC   __zpabi_f2__p0_2
   STA   __local_f0__l1_0
   LDA   __local_f0__l1_1
   ADC   __local_f0__2
   STA   __local_f0__l1_1
   LDA   __local_f0__l1_2
   ADC   __zpabi_f2__p3_0
   STA   __local_f0__l1_2
   LDA   __local_f0__l1_3
   ADC   __local_f0__0
   STA   __local_f0__l1_3
   INC   __local_f0__it_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g0+1
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   __zpabi_f2__p3_0
   STA   __local_f0__0
   LDA   __local_f0__l1_0
   SEC
   SBC   g0
   STA   __zpabi_f2__p0_2
   LDA   __local_f0__l1_1
   SBC   g0+1
   STA   __local_f0__2
   LDA   __local_f0__l1_2
   SBC   __zpabi_f2__p3_0
   STA   __zpabi_f2__p3_0
   LDA   __local_f0__l1_3
   SBC   __local_f0__0
   STA   __local_f0__0
   LDA   __local_f0__6
   SEC
   SBC   __zpabi_f2__p0_2
   STA   HARGS+8
   LDA   #$00
   SBC   __local_f0__2
   STA   HARGS+9
   LDA   #$00
   SBC   __zpabi_f2__p3_0
   STA   HARGS+10
   LDA   #$00
   SBC   __local_f0__0
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$DD
   STA   __zpabi_f0__p1
   LDX   #$00
   LDA   #$6B
   JSR   f0
   LDA   g0+1
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g1
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__1
   LDA   __local_main__4
   EOR   __local_main__0
   STA   __local_main__0
   LDA   HARGS+8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   EOR   __local_main__1
   STA   HARGS+2
   LDA   HARGS+11
   EOR   __local_main__0
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS
   LDA   __local_main__2
   STA   HARGS+1
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__3
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__2
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$09
   STA   g0
   LDA   #$25
   STA   g1
   RTS
