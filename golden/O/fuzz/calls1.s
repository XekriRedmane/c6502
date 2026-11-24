SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
g2	EQU	$28
__zpabi_f0__p1_0	EQU	$80
__zpabi_f1__p1_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f1__p1_1	EQU	$81
__zpabi_f0__p2_0	EQU	$82
__zpabi_f1__p1_2	EQU	$82
__zpabi_f0__p2_1	EQU	$83
__zpabi_f1__p1_3	EQU	$83
__local_main__0	EQU	$84
__zpabi_f1__p2	EQU	$84
__local_main__1	EQU	$85
__zpabi_f2__pp_0	EQU	$85
__local_main__2	EQU	$86
__zpabi_f2__pp_1	EQU	$86
__local_f1__0	EQU	$87
__local_main__3	EQU	$87
__local_f1__1	EQU	$88
__local_main__4	EQU	$88
__local_main__5	EQU	$89
__local_f1__p0_0	EQU	$8A
__local_main__6	EQU	$8A
__local_f1__box2	EQU	$8B
__local_main__8	EQU	$8C
__local_main__9	EQU	$8D
__local_f2__0	EQU	$8E
__local_main__10	EQU	$8E
__local_f2__1	EQU	$8F
__local_main__11	EQU	$8F
__local_f2__2	EQU	$90
__local_f2__3	EQU	$91
__local_f0__0	EQU	$92
__local_f0__it_1	EQU	$93
__local_f0__it_0	EQU	$94
__local_f0__l1	EQU	$95
__local_f0__p0	EQU	$96
__local_f3__0	EQU	$97
__local_f3__1	EQU	$98

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p2_0,__zpabi_f0__p2_1 locals=__local_f0__0,__local_f0__it_1,__local_f0__it_0,__local_f0__l1,__local_f0__p0 indirect=false in_cycle=false param_regs=A,,,,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p2 locals=__local_f1__0,__local_f1__1,__local_f1__p0_1,__local_f1__p0_0,__local_f1__box2,__local_f1__3,__local_f1__4 indirect=false in_cycle=false param_regs=A,X,,,,, changes=__zpabi_f1__p1_0 carry=true
; def f2 params=__zpabi_f2__p0,__zpabi_f2__p1,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false param_regs=A,X,,
; def f3 params= locals=__local_f3__0,__local_f3__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f0 -> f3
; call f1 -> f2
; call f1 -> f3
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$41 weight=9
; zp g1 size=1 at=auto def=true init=$7E weight=3
; zp g2 size=2 at=auto def=true init=$78,$00 weight=6
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f3:
   SUBROUTINE

   LDA   g2
   ORA   #$01
   STA   HARGS+2
   LDA   g2+1
   STA   HARGS+3
   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+4
   ORA   #$01
   STA   __local_f3__1
   LDA   #$E8
   EOR   __local_f3__1
   STA   __local_f3__1
   LDA   #$03
   EOR   HARGS+5
   STA   __local_f3__0
   LDA   #$E8
   CLC
   ADC   __local_f3__1
   STA   HARGS+8
   LDA   #$03
   ADC   __local_f3__0
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11
   RTS

f2:
   SUBROUTINE

   LDA   g0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__0
   LDA   #$FF
   SEC
   SBC   g0
   LDA   #$00
   SBC   __local_f2__0
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f2__2
   LDA   g2
   CLC
   ADC   g2
   STA   __local_f2__1
   LDA   __local_f2__2
   EOR   __local_f2__1
   STA   __local_f2__3
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__0
   LDA   __local_f2__3
   AND   #$07
   TAX
   LDA   g2
   STA   __local_f2__2
   LDA   g2+1
   STA   __local_f2__3
   CPX   #$00
   BEQ   .shift_done@4
.shift_loop@3:
   ASL   __local_f2__2
   ROL   __local_f2__3
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDA   __local_f2__1
   CLC
   ADC   __local_f2__2
   STA   __local_f2__1
   LDA   __local_f2__0
   ADC   __local_f2__3
   LDY   #$01
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   DEY
   STA   (__zpabi_f2__pp_0),Y
   LDA   #$01
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0_0
   JSR   f3
   LDA   __zpabi_f1__p1_0
   EOR   HARGS+8
   STA   __local_f1__1
   LDA   __zpabi_f1__p1_1
   EOR   HARGS+9
   STA   __local_f1__0
   LDA   g0
   CLC
   ADC   __local_f1__1
   STA   g0
   LDA   __local_f1__1
   STA   __local_f1__box2
   LDA   __local_f1__0
   STA   __local_f1__box2+1
   LDA   __local_f1__p0_0
   AND   #$07
   TAX
   BEQ   .shift_done@6
.shift_loop@5:
   LSR   __zpabi_f1__p1_0
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   #<__local_f1__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f1__box2
   STA   __zpabi_f2__pp_1
   LDX   __zpabi_f1__p1_0
   LDA   #$07
   JSR   f2
   LDA   g0
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   ORA   g0
   BEQ   .cmp_end@9
   SEC
   RTS
.cmp_end@9:
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   LDA   g0
   BEQ   .vthread@0
   BMI   .sx_neg@10
.vthread@0:
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   HARGS+1
   LDA   __zpabi_f0__p2_0
   ORA   #$01
   STA   HARGS+2
   LDA   __zpabi_f0__p2_1
   STA   HARGS+3
   LDA   g0
   STA   HARGS
   JSR   udivmod16
   LDA   HARGS+6
   STA   __local_f0__l1
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
   BVC   .jcmp_novf@12
   EOR   #$80
.jcmp_novf@12:
   BPL   .loop@0_break
   JSR   f3
   LDA   HARGS+8
   CLC
   ADC   __local_f0__it_0
   CLC
   ADC   __local_f0__l1
   STA   __local_f0__l1
   INC   __local_f0__it_0
   BNE   .loop@0_start
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   CLC
   ADC   __local_f0__l1
   STA   g1
   LDA   __local_f0__p0
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_f0__0
   LDA   __local_f0__p0
   SEC
   SBC   #$E8
   LDA   __local_f0__0
   SBC   #$03
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   ORA   __local_f0__p0
   RTS

main:
   SUBROUTINE

   LDA   #$32
   STA   __zpabi_f0__p1_0
   LDA   #$69
   STA   __zpabi_f0__p2_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p2_1
   LDA   #$B1
   JSR   f0
   STA   __local_main__11
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_main__10
   STA   __local_main__9
   STA   __local_main__8
   LDA   g0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_main__6
   STA   __local_main__5
   STA   __local_main__4
   LDA   g1
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__6
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__5
   LDA   __local_main__4
   EOR   __local_main__0
   STA   __local_main__4
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__2
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   STA   __local_main__1
   LDA   __local_main__4
   STA   __local_main__0
   LDA   __local_main__11
   EOR   __local_main__3
   STA   __local_main__3
   LDA   __local_main__10
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__9
   EOR   __local_main__1
   STA   HARGS+2
   LDA   __local_main__8
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
   STA   g2+1
   LDA   #$41
   STA   g0
   LDA   #$7E
   STA   g1
   LDA   #$78
   STA   g2
   RTS
