SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__pp_0	EQU	$80
__zpabi_f1__p1	EQU	$80
__zpabi_f3__p0_0	EQU	$80
__zpabi_f4__p1_0	EQU	$80
__zpabi_f5__p0_0	EQU	$80
__local_f2__1	EQU	$81
__zpabi_f0__pp_1	EQU	$81
__zpabi_f1__p2	EQU	$81
__zpabi_f3__p0_1	EQU	$81
__zpabi_f4__p1_1	EQU	$81
__zpabi_f5__p0_1	EQU	$81
__local_f1__0	EQU	$82
__zpabi_f3__p0_2	EQU	$82
__zpabi_f4__p1_2	EQU	$82
__zpabi_f5__p0_2	EQU	$82
__local_f1__1	EQU	$83
__local_f2__3	EQU	$83
__zpabi_f3__p0_3	EQU	$83
__zpabi_f4__p1_3	EQU	$83
__zpabi_f5__p0_3	EQU	$83
__local_f1__2	EQU	$84
__local_f2__pp	EQU	$84
__local_main__mbox	EQU	$84
__zpabi_f3__p1_0	EQU	$84
__zpabi_f4__p2_0	EQU	$84
__zpabi_f5__p1_0	EQU	$84
__local_f1__3	EQU	$85
__local_f2__4	EQU	$85
__zpabi_f3__p1_1	EQU	$85
__zpabi_f4__p2_1	EQU	$85
__zpabi_f5__p1_1	EQU	$85
__local_f0__0	EQU	$86
__local_f1__4	EQU	$86
__zpabi_f3__p2_0	EQU	$86
__zpabi_f5__p1_2	EQU	$86
__local_f0__1	EQU	$87
__local_f1__p0_1	EQU	$87
__zpabi_f3__p2_1	EQU	$87
__zpabi_f5__p1_3	EQU	$87
__local_f0__2	EQU	$88
__local_f1__p0_0	EQU	$88
__zpabi_f3__p3	EQU	$88
__zpabi_f5__p2_0	EQU	$88
__zpabi_f5__p2_1	EQU	$89
__local_f0__4	EQU	$8A
__zpabi_f5__p2_2	EQU	$8A
__local_f0__5	EQU	$8B
__zpabi_f5__p3_0	EQU	$8B
__local_f0__6	EQU	$8C
__zpabi_f5__p3_1	EQU	$8C
__local_f0__p0_1	EQU	$8D
__zpabi_f5__pp_0	EQU	$8D
__local_f0__p0_0	EQU	$8E
__zpabi_f5__pp_1	EQU	$8E
__local_f5__0	EQU	$8F
__local_f5__1	EQU	$90
__local_f5__2	EQU	$91
__local_f5__3	EQU	$92
__local_f5__4	EQU	$93
__local_f5__5	EQU	$94
__local_f5__6	EQU	$95
__local_f5__7	EQU	$96

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__p0_1,__local_f0__p0_0 indirect=false in_cycle=false param_regs=A,X,, carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1,__zpabi_f1__p2 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__p0_1,__local_f1__p0_0 indirect=false in_cycle=false param_regs=A,X,,
; def f2 params=__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__pp,__local_f2__4 indirect=false in_cycle=false param_regs=A,X
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p3 locals=__local_f3__0,__local_f3__1 indirect=false in_cycle=false
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p1_2,__zpabi_f4__p1_3,__zpabi_f4__p2_0,__zpabi_f4__p2_1 locals=__local_f4__0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def f5 params=__zpabi_f5__p0_0,__zpabi_f5__p0_1,__zpabi_f5__p0_2,__zpabi_f5__p0_3,__zpabi_f5__p1_0,__zpabi_f5__p1_1,__zpabi_f5__p1_2,__zpabi_f5__p1_3,__zpabi_f5__p2_0,__zpabi_f5__p2_1,__zpabi_f5__p2_2,__zpabi_f5__p3_0,__zpabi_f5__p3_1,__zpabi_f5__pp_0,__zpabi_f5__pp_1 locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3,__local_f5__4,__local_f5__5,__local_f5__6,__local_f5__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__mbox,__local_main__3 indirect=false in_cycle=false
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$09 weight=11
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f5:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_f5__pp_0),Y
   CLC
   ADC   __zpabi_f5__p2_0
   STA   __local_f5__1
   INY
   LDA   (__zpabi_f5__pp_0),Y
   ADC   __zpabi_f5__p2_1
   STA   (__zpabi_f5__pp_0),Y
   LDA   __local_f5__1
   DEY
   STA   (__zpabi_f5__pp_0),Y
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__3
   INY
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f5__1
   STA   __local_f5__0
   LDA   __local_f5__3
   SEC
   SBC   __zpabi_f5__p0_0
   STA   __local_f5__7
   LDA   __local_f5__2
   SBC   __zpabi_f5__p0_1
   STA   __local_f5__6
   LDA   __local_f5__1
   SBC   __zpabi_f5__p0_2
   STA   __local_f5__5
   LDA   __local_f5__0
   SBC   __zpabi_f5__p0_3
   STA   __local_f5__4
   LDA   __zpabi_f5__p2_0
   ORA   __zpabi_f5__p2_1
   ORA   __zpabi_f5__p2_2
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f5__3
   LDA   #$00
   BEQ   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f5__3
.and_end@1:
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_f5__1
   STA   __local_f5__0
   LDA   __local_f5__7
   SEC
   SBC   __local_f5__3
   LDA   __local_f5__6
   SBC   #$00
   LDA   __local_f5__5
   SBC   __local_f5__1
   LDA   __local_f5__4
   SBC   __local_f5__0
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   STA   __local_f5__0
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f5__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f4:
   SUBROUTINE

   LDA   __zpabi_f4__p2_1
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDX   __zpabi_f4__p2_0
   STX   HARGS+8
   LDX   __zpabi_f4__p2_1
   STX   HARGS+9
   AND   #$07
   STA   HARGS+10
   RTS

f3:
   SUBROUTINE

   LDA   __zpabi_f3__p1_0
   STA   HARGS
   LDA   __zpabi_f3__p1_1
   STA   HARGS+1
   LDA   #$65
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   SEC
   LDA   #$00
   SBC   HARGS+4
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__pp
   STX   __local_f2__4
   LDA   g0
   STA   __local_f2__3
   LDY   #$01
   LDA   (__local_f2__pp),Y
   STA   __local_f2__1
   DEY
   LDA   (__local_f2__pp),Y
   CLC
   ADC   g0
   BCC   .carry_inc@0
   INC   __local_f2__1
.carry_inc@0:
   LDY   #$00
   STA   (__local_f2__pp),Y
   LDA   __local_f2__1
   INY
   STA   (__local_f2__pp),Y
   LDA   __local_f2__3
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0_0
   STX   __local_f1__p0_1
   LDA   __zpabi_f1__p1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_f1__4
   LDA   __zpabi_f1__p2
   AND   #$07
   TAX
   LDA   __zpabi_f1__p1
   STA   __local_f1__2
   LDA   __local_f1__4
   STA   __local_f1__3
   CPX   #$00
   BEQ   .shift_done@12
.shift_loop@11:
   ASL   __local_f1__2
   ROL   __local_f1__3
   DEX
   BNE   .shift_loop@11
.shift_done@12:
   LDA   g0
   CLC
   ADC   __zpabi_f1__p1
   STA   __local_f1__1
   LDA   #$00
   ADC   __local_f1__4
   STA   __local_f1__0
   LDA   __local_f1__2
   SEC
   SBC   __local_f1__1
   LDA   __local_f1__3
   SBC   __local_f1__0
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   AND   #$7F
   STA   __local_f1__0
   LDA   __local_f1__p0_0
   ORA   __local_f1__p0_1
   BEQ   .and_false@2
   LDA   __local_f1__p0_0
   ORA   __local_f1__p0_1
   BEQ   .and_false@2
   LDA   __local_f1__0
   BEQ   .and_false@2
   LDA   #$01
   RTS
.and_false@2:
   LDA   #$00
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   STX   __local_f0__p0_1
   STA   HARGS
   STX   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   LDA   #$7F
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   __local_f0__5
   LDA   HARGS+7
   STA   __local_f0__6
   LDA   __local_f0__p0_0
   EOR   g0
   AND   #$07
   TAX
   BEQ   .shift_done@15
.shift_loop@14:
   ASL   __local_f0__5
   ROL   __local_f0__6
   DEX
   BNE   .shift_loop@14
.shift_done@15:
   LDA   #$00
   CMP   __local_f0__p0_1
   BNE   .cmp_true@16
   LDA   g0
   CMP   __local_f0__p0_0
   BNE   .cmp_true@16
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __local_f0__2
   LDA   g0
   BNE   .or_true@6
   LDA   __local_f0__5
   ORA   __local_f0__6
   BNE   .or_true@6
   LDA   #$00
   STA   __local_f0__1
   JMP   .vthread@0
.or_true@6:
   LDA   #$01
   STA   __local_f0__1
   LDA   #$00
   BNE   .thread@0
.vthread@0:
   LDA   __local_f0__2
   CMP   __local_f0__1
   BEQ   .cmp_true@19
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   STA   __local_f0__4
   LDA   __local_f0__5
   CLC
   ADC   __local_f0__p0_0
   STA   __local_f0__1
   LDA   __local_f0__6
   ADC   __local_f0__p0_1
   STA   __local_f0__0
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   CLC
   ADC   __local_f0__1
   STA   __local_f0__1
   INY
   LDA   (__zpabi_f0__pp_0),Y
   ADC   __local_f0__0
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__1
   DEY
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__4
   BEQ   .cmp_end@22
   SEC
   RTS
.cmp_end@22:
   CMP   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   LDX   #$01
   LDA   #$09
   JSR   f0
   LDA   #$00
   ROL
   EOR   g0
   LDX   #$00
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$09
   STA   g0
   RTS
