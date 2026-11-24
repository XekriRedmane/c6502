SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$28
__zpabi_f0__p1_0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__zpabi_f3__p1_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__zpabi_f3__p1_1	EQU	$81
__local_f3__p0	EQU	$82
__zpabi_f0__p2_0	EQU	$82
__zpabi_f2__p0_2	EQU	$82
__local_f3__0	EQU	$83
__zpabi_f0__p2_1	EQU	$83
__zpabi_f2__p0_3	EQU	$83
__zpabi_f0__p3_0	EQU	$84
__zpabi_f2__p1_0	EQU	$84
__zpabi_f0__p3_1	EQU	$85
__zpabi_f2__p1_1	EQU	$85
__zpabi_f0__p3_2	EQU	$86
__zpabi_f2__p1_2	EQU	$86
__zpabi_f0__p3_3	EQU	$87
__zpabi_f2__p1_3	EQU	$87
__local_main__0	EQU	$88
__zpabi_f4__p1_0	EQU	$88
__zpabi_f4__p1_1	EQU	$89
__local_main__2	EQU	$8A
__zpabi_f4__p1_2	EQU	$8A
__local_main__3	EQU	$8B
__zpabi_f4__p1_3	EQU	$8B
__local_main__4	EQU	$8C
__zpabi_f4__p2_0	EQU	$8C
__zpabi_f4__p2_1	EQU	$8D
__zpabi_f4__pp_0	EQU	$8E
__local_f0__0	EQU	$8F
__zpabi_f4__pp_1	EQU	$8F
__local_f0__1	EQU	$90
__local_f2__0	EQU	$90
__local_f0__2	EQU	$91
__local_f2__1	EQU	$91
__local_f0__3	EQU	$92
__local_f2__2	EQU	$92
__local_f2__6	EQU	$96
__local_f0__8	EQU	$97
__local_f2__7	EQU	$97
__local_f0__9	EQU	$98
__local_f2__box4	EQU	$98
__local_f0__p0	EQU	$99
__local_f4__0	EQU	$9A
__local_f4__1	EQU	$9B
__local_f4__p0	EQU	$9C

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p3_0,__zpabi_f0__p3_1,__zpabi_f0__p3_2,__zpabi_f0__p3_3 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__8,__local_f0__9,__local_f0__p0 indirect=false in_cycle=false param_regs=A,,,,,,,, carry=true
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__box4,__local_f2__9 indirect=false in_cycle=false carry=true
; def f3 params=__zpabi_f3__p0,__zpabi_f3__p1_0,__zpabi_f3__p1_1 locals=__local_f3__p0,__local_f3__0 indirect=false in_cycle=false param_regs=A,,
; def f4 params=__zpabi_f4__p0,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p1_2,__zpabi_f4__p1_3,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__0,__local_f4__1,__local_f4__p0 indirect=false in_cycle=false param_regs=A,,,,,,,, carry=true
; def f5 params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; call f2 -> f4
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item f0@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$7A,$00 weight=8
; zp g0 size=4 at=auto def=true init=$88,$00,$00,$00 weight=12
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f5:
   SUBROUTINE

   ; prologue: 6 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$08
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f4:
   SUBROUTINE

   STA   __local_f4__p0
   SEC
   LDY   #$00
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__0
   INY
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__1
   INC   __local_f4__0
   BNE   .inc_done@0
   INC   __local_f4__1
.inc_done@0:
   LDA   __local_f4__0
   LDY   #$00
   STA   (__zpabi_f4__pp_0),Y
   LDA   __local_f4__1
   INY
   STA   (__zpabi_f4__pp_0),Y
   LDA   __zpabi_f4__p2_0
   ORA   __local_f4__p0
   SBC   #$E8
   LDA   __zpabi_f4__p2_1
   SBC   #$03
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__p0
   LDA   g0
   AND   g0
   STA   HARGS
   LDA   g0+1
   AND   g0+1
   STA   HARGS+1
   LDA   g0+2
   AND   g0+2
   STA   HARGS+2
   LDA   g0+3
   AND   g0+3
   STA   HARGS+3
   LDA   __local_f3__p0
   ORA   #$01
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+12
   SEC
   SBC   #$08
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .or_true@2
   LDA   g0
   BEQ   .or_end@3
.or_true@2:
.or_end@3:
   SEC
   LDA   #$00
   SBC   g0
   STA   __local_f3__0
   LDA   #$00
   SBC   g0+1
   STA   __local_f3__p0
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__0
   STA   HARGS+8
   LDA   __local_f3__p0
   STA   HARGS+9
   RTS

f2:
   SUBROUTINE

   LDA   g0
   SEC
   SBC   g1
   LDA   g0+1
   SBC   g1+1
   LDA   g0+2
   SBC   #$00
   LDA   g0+3
   SBC   #$00
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f2__2
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __zpabi_f2__p1_0
   SEC
   SBC   __local_f2__2
   STA   __local_f2__7
   LDA   __zpabi_f2__p1_1
   SBC   #$00
   STA   __local_f2__6
   LDA   __zpabi_f2__p1_2
   SBC   __local_f2__1
   STA   __local_f2__1
   LDA   __zpabi_f2__p1_3
   SBC   __local_f2__0
   STA   __local_f2__0
   LDA   __local_f2__7
   STA   __local_f2__box4
   LDA   __local_f2__6
   STA   __local_f2__box4+1
   LDA   __zpabi_f2__p0_0
   AND   #$07
   TAX
   LDA   __local_f2__7
   STA   __zpabi_f4__p1_0
   LDA   #$00
   STA   __zpabi_f4__p1_1
   CPX   #$00
   BEQ   .shift_done@7
.shift_loop@6:
   ASL   __zpabi_f4__p1_0
   ROL   __zpabi_f4__p1_1
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   __zpabi_f4__p1_1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_f4__p1_2
   LDA   __local_f2__7
   ORA   __local_f2__6
   ORA   __local_f2__1
   ORA   __local_f2__0
   BEQ   .lnot_true@10
   LDA   #$00
   BEQ   .lnot_end@11
.lnot_true@10:
   LDA   #$01
.lnot_end@11:
   STA   __zpabi_f4__p2_0
   LDA   __zpabi_f4__p1_2
   STA   __zpabi_f4__p1_3
   LDA   #$00
   STA   __zpabi_f4__p2_1
   LDA   #<__local_f2__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f2__box4
   STA   __zpabi_f4__pp_1
   LDA   __local_f2__7
   JSR   f4
   SEC
   RTS

f1:
   SUBROUTINE

   ; prologue: 6 arg bytes, 1 local bytes, 1 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$02
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y

   LDA   #$07
   SEC
   LDY   #$04
   SBC   (FP),Y
   STA   $87
   LDA   #$00
   INY
   SBC   (FP),Y
   STA   $86
   LDA   #$00
   INY
   SBC   (FP),Y
   STA   $85
   LDA   #$00
   INY
   SBC   (FP),Y
   STA   $84
   LDA   g0
   STA   $83
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   $82
   STA   $81
   STA   $80
   LDA   $83
   SEC
   SBC   $87
   LDA   $82
   SBC   $86
   LDA   $81
   SBC   $85
   LDA   $80
   SBC   $84
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $87
   LDY   #$04
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   $87
   BNE   .or_true@4
   LDA   #$00
   STA   $81
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   STA   $81
.or_end@5:
   LDA   g0
   CLC
   ADC   $81
   STA   $C0
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDA   g1
   STA   $81
   LDA   g1+1
   STA   $80
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   $80
   STA   $80
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f5
   STA   $85
   LDA   #>f5
   STA   $86
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   LDA   #$E8
   ORA   $81
   STA   $84
   LDA   #$03
   ORA   $80
   STA   $83
   LDA   $87
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $82
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   $87
   SEC
   SBC   $81
   STA   $81
   LDA   $82
   SBC   $80
   STA   $80
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$04
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $85
   STA   DPTR
   LDA   $86
   STA   DPTR+1
   JSR   icall
   LDA   #$64
   AND   $C0
   STA   $84
   LDA   $C0
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   $82
   STA   $81
   STA   $80
   LDY   #$04
   LDA   (FP),Y
   CLC
   ADC   $C0
   STA   $83
   INY
   LDA   (FP),Y
   ADC   $82
   STA   $82
   INY
   LDA   (FP),Y
   ADC   $81
   STA   $81
   INY
   LDA   (FP),Y
   ADC   $80
   STA   $80
   LDA   $84
   EOR   $83
   STA   $83
   LDA   #$00
   EOR   $82
   STA   $82
   LDA   #$00
   EOR   $81
   STA   $81
   LDA   #$00
   EOR   $80
   ORA   $81
   ORA   $82
   ORA   $83
   BEQ   .cmp_end@19
   LDA   #$01
.cmp_end@19:

   ; epilogue
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   LDA   __zpabi_f0__p1_1
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   HARGS+2
   STA   HARGS+3
   LDA   #$01
   ORA   g0
   JSR   f0@run0
   LDA   __zpabi_f0__p1_0
   STA   HARGS
   LDA   __zpabi_f0__p1_1
   STA   HARGS+1
   JSR   mul32
   LDA   __zpabi_f0__p1_1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_f0__9
   STA   __local_f0__8
   LDA   g0
   ORA   #$01
   JSR   f0@run0
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   JSR   udivmod32
   LDA   __local_f0__p0
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_f0__2
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __local_f0__p0
   SEC
   SBC   __zpabi_f0__p1_0
   STA   __local_f0__3
   LDA   __local_f0__2
   SBC   __zpabi_f0__p1_1
   STA   __local_f0__2
   LDA   __local_f0__1
   SBC   __local_f0__9
   STA   __local_f0__1
   LDA   __local_f0__0
   SBC   __local_f0__8
   STA   __local_f0__0
   LDA   HARGS+12
   SEC
   SBC   __local_f0__3
   LDA   HARGS+13
   SBC   __local_f0__2
   LDA   HARGS+14
   SBC   __local_f0__1
   LDA   HARGS+15
   SBC   __local_f0__0
   RTS

f0@run0:
   SUBROUTINE

   STA   HARGS+4
   LDA   g0+1
   STA   HARGS+5
   LDA   g0+2
   STA   HARGS+6
   LDA   g0+3
   STA   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$C3
   STA   __zpabi_f0__p1_0
   LDA   #$A1
   STA   __zpabi_f0__p2_0
   LDA   #$26
   STA   __zpabi_f0__p3_0
   LDA   #$01
   STA   __zpabi_f0__p3_1
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p3_2
   STA   __zpabi_f0__p3_3
   LDA   #$25
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__4
   EOR   __local_main__3
   STA   __local_main__3
   LDA   #$00
   EOR   __local_main__2
   STA   __local_main__2
   LDA   #$00
   EOR   g0+2
   STA   HARGS+2
   LDA   #$00
   EOR   g0+3
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
   STA   g0+2
   STA   g0+3
   STA   g1+1
   LDA   #$88
   STA   g0
   LDA   #$7A
   STA   g1
   RTS
