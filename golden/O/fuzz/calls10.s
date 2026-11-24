SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_f0__0	EQU	$8E
__local_f0__1	EQU	$8F
__local_f0__2	EQU	$90
__local_f0__3	EQU	$91
__local_f0__it_1	EQU	$92
__local_f0__it_0	EQU	$93
__local_f0__l0_3	EQU	$94
__local_f0__l0_2	EQU	$95
__local_f0__l0_1	EQU	$96
__local_f0__l0_0	EQU	$97
__local_f0__4	EQU	$98
__local_f0__p0	EQU	$99
__local_f0__box1	EQU	$9A
__local_f1__0	EQU	$9F
__local_f1__1	EQU	$A0
__local_f1__2	EQU	$A1
__local_f1__3	EQU	$A2
__local_f1__pp	EQU	$A3
__local_f1__4	EQU	$A4
__local_f1__5	EQU	$A5
__local_f1__6	EQU	$A6
__local_f1__7	EQU	$A7
__local_f1__8	EQU	$A8
__local_f1__9	EQU	$A9
__local_f1__10	EQU	$AA
__local_f1__11	EQU	$AB
__local_f2__0	EQU	$AE
__local_f2__1	EQU	$AF
__local_f2__5	EQU	$B3
__local_f2__6	EQU	$B4
__local_f2__7	EQU	$B5
__local_f2__13	EQU	$BB
__local_f2__14	EQU	$BC
__local_f2__22	EQU	$C4
__local_f2__23	EQU	$C5

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__it_1,__local_f0__it_0,__local_f0__l0_3,__local_f0__l0_2,__local_f0__l0_1,__local_f0__l0_0,__local_f0__4,__local_f0__p0,__local_f0__box1,__local_f0__6,__local_f0__7,__local_f0__8,__local_f0__9 indirect=false in_cycle=false param_regs=A
; def f1 params=__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__pp,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__9,__local_f1__10,__local_f1__11,__local_f1__12,__local_f1__13 indirect=false in_cycle=false param_regs=A,X carry=true
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__8,__local_f2__9,__local_f2__10,__local_f2__11,__local_f2__12,__local_f2__13,__local_f2__14,__local_f2__15,__local_f2__16,__local_f2__17,__local_f2__18,__local_f2__19,__local_f2__20,__local_f2__21,__local_f2__22,__local_f2__23,__local_f2__24 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call f1 -> f2
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$34 weight=5
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f2:
   SUBROUTINE

   LDA   g1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__5
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   CMP   g0+6
   BNE   .thread@0
   CMP   g0+5
   BNE   .thread@0
   CMP   g0+4
   BNE   .thread@0
   CMP   g0+3
   BNE   .thread@0
   CMP   g0+2
   BNE   .thread@0
   LDA   __local_f2__5
   CMP   g0+1
   BNE   .thread@0
   LDA   g1
   CMP   g0
   BEQ   .cmp_true@4
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   __local_f2__23
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __local_f2__22
   LDA   g0+6
   AND   #$1F
   STA   __local_f2__14
   LDA   g0+6
   AND   #$1F
   STA   __local_f2__0
   LDA   g0
   CLC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   LDA   g0+4
   SBC   g0+4
   LDA   g0+5
   SBC   g0+5
   LDA   __local_f2__0
   SBC   __local_f2__14
   LDA   #$00
   ROL
   STA   __local_f2__13
   SEC
   LDA   #$00
   SBC   g0
   STA   __local_f2__6
   LDA   __local_f2__13
   SEC
   SBC   __local_f2__6
   STA   __local_f2__7
   LDA   __local_f2__23
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f2__22
   STA   HARGS+6
   STA   HARGS+7
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+15
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   CMP   g0+6
   BNE   .thread@1
   CMP   g0+5
   BNE   .thread@1
   CMP   g0+4
   BNE   .thread@1
   LDA   HARGS+15
   CMP   g0+3
   BNE   .thread@1
   LDA   HARGS+14
   CMP   g0+2
   BNE   .thread@1
   LDA   HARGS+13
   CMP   g0+1
   BNE   .thread@1
   LDA   HARGS+12
   CMP   g0
   BEQ   .cmp_true@13
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   AND   __local_f2__23
   STA   __local_f2__1
   LDA   __local_f2__23
   AND   __local_f2__7
   AND   __local_f2__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__pp
   STX   __local_f1__4
   LDA   g0
   SEC
   SBC   #$03
   LDA   g0+1
   SBC   #$00
   LDA   g0+2
   SBC   #$00
   LDA   g0+3
   SBC   #$00
   LDA   g0+4
   SBC   #$00
   LDA   g0+5
   SBC   #$00
   LDA   g0+6
   SBC   #$00
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f1__3
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __local_f1__2
   STA   __local_f1__0
   LDA   #$00
   CMP   g0+6
   BNE   .cmp_true@19
   CMP   g0+5
   BNE   .cmp_true@19
   CMP   g0+4
   BNE   .cmp_true@19
   LDA   __local_f1__0
   CMP   g0+3
   BNE   .cmp_true@19
   CMP   g0+2
   BNE   .cmp_true@19
   LDA   #$00
   CMP   g0+1
   BNE   .cmp_true@19
   LDA   __local_f1__3
   CMP   g0
   BNE   .cmp_true@19
   LDA   #$00
   BEQ   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   STA   __local_f1__1
   LDA   __local_f1__3
   SEC
   SBC   __local_f1__3
   LDA   #$00
   SBC   #$00
   LDA   __local_f1__2
   SBC   __local_f1__2
   LDA   __local_f1__0
   SBC   __local_f1__0
   LDA   #$00
   ROL
   STA   __local_f1__0
   LDA   #$00
   BNE   .cmp_true@22
   LDA   __local_f1__1
   CMP   __local_f1__0
   BNE   .cmp_true@22
   LDA   #$00
   BEQ   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   STA   __local_f1__11
   LDA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   __local_f1__10
   STA   __local_f1__9
   STA   __local_f1__8
   STA   __local_f1__7
   STA   __local_f1__6
   LDY   #$00
   LDA   (__local_f1__pp),Y
   STA   __local_f1__2
   INY
   LDA   (__local_f1__pp),Y
   STA   __local_f1__1
   LDA   #$64
   CMP   __local_f1__3
   BEQ   .cmp_true@27
   LDA   #$00
   BEQ   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   CLC
   ADC   __local_f1__2
   BCC   .carry_inc@0
   INC   __local_f1__1
.carry_inc@0:
   LDY   #$00
   STA   (__local_f1__pp),Y
   LDA   __local_f1__1
   INY
   STA   (__local_f1__pp),Y
   JSR   f2
   LDY   #$00
   LDA   (__local_f1__pp),Y
   BEQ   .lb_skip@0
   JMP   .or_true@0
.lb_skip@0:
   LDA   g1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_f1__4
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_f1__pp
   STA   __local_f1__3
   STA   __local_f1__2
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   __local_f1__11
   SEC
   SBC   g1
   STA   __local_f1__5
   LDA   #$00
   SBC   __local_f1__4
   STA   __local_f1__4
   LDA   __local_f1__10
   SBC   __local_f1__pp
   STA   __local_f1__pp
   LDA   __local_f1__9
   SBC   __local_f1__3
   STA   __local_f1__3
   LDA   __local_f1__8
   SBC   __local_f1__2
   STA   __local_f1__2
   LDA   __local_f1__7
   SBC   __local_f1__1
   STA   __local_f1__1
   LDA   __local_f1__6
   SBC   __local_f1__0
   AND   #$01
   EOR   #$01
   STA   __local_f1__0
   DEC   __local_f1__0
   LDA   __local_f1__5
   ORA   __local_f1__4
   ORA   __local_f1__pp
   ORA   __local_f1__3
   ORA   __local_f1__2
   ORA   __local_f1__1
   ORA   __local_f1__0
   BNE   .or_true@0
   CLC
   RTS
.or_true@0:
   SEC
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   LDA   g0
   STA   __local_f0__l0_2
   LDA   g0+1
   STA   __local_f0__l0_3
   LDA   g0+2
   STA   __local_f0__it_0
   LDA   g0+3
   STA   __local_f0__it_1
   JSR   f2
   LDA   __local_f0__l0_2
   EOR   HARGS+8
   STA   __local_f0__l0_0
   LDA   __local_f0__l0_3
   EOR   HARGS+9
   STA   __local_f0__l0_1
   LDA   __local_f0__it_0
   EOR   HARGS+10
   STA   __local_f0__l0_2
   LDA   __local_f0__it_1
   EOR   HARGS+11
   STA   __local_f0__l0_3
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __local_f0__2
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g0
   CLC
   ADC   __local_f0__l0_0
   STA   __local_f0__4
   LDA   g0+1
   ADC   __local_f0__l0_1
   STA   __local_f0__it_0
   LDA   g0+2
   ADC   __local_f0__l0_2
   STA   __local_f0__it_1
   LDA   g0+3
   ADC   __local_f0__l0_3
   STA   __local_f0__3
   LDA   g0+4
   ADC   __local_f0__2
   STA   __local_f0__2
   LDA   g0+5
   ADC   __local_f0__1
   STA   __local_f0__1
   LDA   g0+6
   ADC   __local_f0__0
   AND   #$01
   EOR   #$01
   LDX   __local_f0__4
   STX   g0
   LDX   __local_f0__it_0
   STX   g0+1
   LDX   __local_f0__it_1
   STX   g0+2
   LDX   __local_f0__3
   STX   g0+3
   LDX   __local_f0__2
   STX   g0+4
   LDX   __local_f0__1
   STX   g0+5
   SEC
   SBC   #$01
   STA   g0+6
   LDA   __local_f0__l0_0
   STA   __local_f0__box1
   LDA   __local_f0__l0_1
   STA   __local_f0__box1+1
   LDA   #$00
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__it_1
.loop@0_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@35
   EOR   #$80
.jcmp_novf@35:
   BPL   .loop@0_break
   LDX   #>__local_f0__box1
   LDA   #<__local_f0__box1
   JSR   f1
   LDA   #$00
   ROL
   CLC
   ADC   __local_f0__it_0
   STA   __local_f0__3
   LDA   #$00
   ADC   __local_f0__it_1
   STA   __local_f0__2
   BMI   .sx_neg@36
   LDA   #$00
   BEQ   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __local_f0__l0_0
   CLC
   ADC   __local_f0__3
   STA   __local_f0__l0_0
   LDA   __local_f0__l0_1
   ADC   __local_f0__2
   STA   __local_f0__l0_1
   LDA   __local_f0__l0_2
   ADC   __local_f0__1
   STA   __local_f0__l0_2
   LDA   __local_f0__l0_3
   ADC   __local_f0__0
   STA   __local_f0__l0_3
   INC   __local_f0__it_0
   BNE   .loop@0_start
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f0__box1
   STA   __local_f0__0
   LDA   __local_f0__box1+1
   STA   __local_f0__3
   BMI   .sx_neg@38
   LDA   #$00
   BEQ   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   STA   __local_f0__2
   STA   __local_f0__1
   LDA   __local_f0__l0_0
   CLC
   ADC   __local_f0__0
   STA   __local_f0__0
   LDA   __local_f0__l0_1
   ADC   __local_f0__3
   STA   HARGS+1
   LDA   __local_f0__l0_2
   ADC   __local_f0__2
   STA   HARGS+2
   LDA   __local_f0__l0_3
   ADC   __local_f0__1
   STA   HARGS+3
   LDA   g1
   CLC
   ADC   __local_f0__0
   STA   g1
   LDA   g0
   AND   #$07
   STA   HARGS+4
   LDA   __local_f0__0
   STA   HARGS
   JSR   asl32
   LDA   __local_f0__p0
   BMI   .sx_neg@40
   LDA   #$00
   BEQ   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   STA   __local_f0__0
   SEC
   LDA   #$00
   SBC   __local_f0__p0
   STA   __local_f0__3
   LDA   #$00
   SBC   __local_f0__0
   STA   __local_f0__2
   BMI   .sx_neg@42
   LDA   #$00
   BEQ   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   HARGS+5
   SEC
   SBC   __local_f0__3
   LDA   HARGS+6
   SBC   __local_f0__2
   LDA   HARGS+7
   SBC   __local_f0__1
   LDA   HARGS+8
   SBC   __local_f0__0
   BVC   .cmp_novf@44
   EOR   #$80
.cmp_novf@44:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f0__0
   LDA   #$00
   BPL   .sx_done@46
   LDA   #$FF
.sx_done@46:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f0__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

main:
   SUBROUTINE

   LDA   #$D1
   JSR   f0
   LDA   g1
   BMI   .sx_neg@47
   LDA   #$00
   BEQ   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   __local_main__2
   STA   __local_main__2
   LDA   g0+2
   EOR   __local_main__1
   STA   __local_main__1
   LDA   g0+3
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

g0:
   DC.B  $93
   DS.B  6

__zp_init:
   SUBROUTINE

   LDA   #$34
   STA   g1
   RTS
