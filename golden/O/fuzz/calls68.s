SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__zpabi_f0__p1_0	EQU	$80
__local_f1__1	EQU	$81
__zpabi_f0__p1_1	EQU	$81
__local_f1__2	EQU	$82
__zpabi_f0__p1_2	EQU	$82
__local_f1__3	EQU	$83
__zpabi_f0__p1_3	EQU	$83
__local_f1__pp	EQU	$84
__zpabi_f4__p0_0	EQU	$84
__local_f1__4	EQU	$85
__zpabi_f4__p0_1	EQU	$85
__zpabi_f4__p2_0	EQU	$86
__zpabi_f4__p2_1	EQU	$87
__zpabi_f4__p2_2	EQU	$88
__zpabi_f4__p2_3	EQU	$89
__zpabi_f4__p3_0	EQU	$8A
__zpabi_f4__p3_1	EQU	$8B
__local_f3__0	EQU	$8C
__local_main__0	EQU	$8C
__local_f3__1	EQU	$8D
__local_main__1	EQU	$8D
__local_f3__2	EQU	$8E
__local_main__2	EQU	$8E
__local_f3__3	EQU	$8F
__local_main__3	EQU	$8F
__local_f3__4	EQU	$90
__local_f3__p0	EQU	$91
__local_f0__0	EQU	$9A
__local_f0__1	EQU	$9B
__local_f0__2	EQU	$9C
__local_f0__3	EQU	$9D
__local_f0__4	EQU	$9E
__local_f0__5	EQU	$9F
__local_f0__6	EQU	$A0
__local_f0__p0_1	EQU	$A1
__local_f0__p0_0	EQU	$A2
__local_f4__0	EQU	$A7
__local_f4__1	EQU	$A8
__local_f4__2	EQU	$A9
__local_f4__3	EQU	$AA

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__p0_1,__local_f0__p0_0,__local_f0__7,__local_f0__8,__local_f0__9,__local_f0__10 indirect=false in_cycle=false param_regs=A,X,,,,
; def f1 params=__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__pp,__local_f1__4 indirect=false in_cycle=false param_regs=A,X
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params=__zpabi_f3__p0 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__p0,__local_f3__5 indirect=false in_cycle=false param_regs=A
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p2_2,__zpabi_f4__p2_3,__zpabi_f4__p3_0,__zpabi_f4__p3_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6,__local_f4__7 indirect=false in_cycle=false param_regs=,,A,X,,,,,, carry=true
; def f5 params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f0 -> f4
; call f3 -> f4
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$9B weight=20
; zp g0 size=4 at=auto def=true init=$80,$00,$00,$00 weight=12
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f5:
   SUBROUTINE

   ; prologue: 10 arg bytes, 0 local bytes
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

   LDY   #$04
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
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

   LDA   g0
   AND   #$07
   STA   HARGS+4
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   g2+2
   STA   HARGS+2
   LDA   g2+3
   STA   HARGS+3
   JSR   asl32
   LDA   #$00
   CMP   g0+3
   BNE   .cmp_end@1
   CMP   g0+2
   BNE   .cmp_end@1
   CMP   g0+1
   BNE   .cmp_end@1
   CMP   g0
   BNE   .cmp_end@1
   LDA   #$01
.cmp_end@1:
   STA   __local_f4__3
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_f4__1
   STA   __local_f4__0
   LDA   #$07
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   LDA   #$00
   SBC   g0+2
   LDA   #$00
   SBC   g0+3
   LDA   #$00
   ROL
   STA   __local_f4__2
   LDA   HARGS+5
   SEC
   SBC   __local_f4__3
   LDA   HARGS+6
   SBC   #$00
   LDA   HARGS+7
   SBC   __local_f4__1
   LDA   HARGS+8
   SBC   __local_f4__0
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f4__1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f4__0
   LDA   __local_f4__1
   SEC
   SBC   __local_f4__2
   LDA   __local_f4__0
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__p0
   LDA   g1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_f3__2
   LDA   g1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f3__0
   LDA   __local_f3__2
   CMP   __local_f3__0
   BNE   .thread@0
   LDA   g1
   CMP   g1
   BEQ   .cmp_true@13
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_f3__4
   LDA   g1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f3__2
   LDA   g1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f3__0
   LDA   __local_f3__2
   CMP   __local_f3__0
   BNE   .cmp_true@20
   LDA   g1
   CMP   g1
   BNE   .cmp_true@20
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   SEC
   SBC   __local_f3__4
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__1
   LDA   __local_f3__p0
   AND   #$64
   STA   __zpabi_f4__p0_0
   LDX   g0+1
   LDA   __local_f3__p0
   ORA   __local_f3__p0
   STA   __zpabi_f4__p3_0
   LDA   #$03
   STA   __zpabi_f4__p2_0
   LDA   #$00
   STA   __zpabi_f4__p0_1
   STA   __zpabi_f4__p2_1
   STA   __zpabi_f4__p2_2
   STA   __zpabi_f4__p2_3
   STA   __zpabi_f4__p3_1
   LDA   g0
   JSR   f4
   LDA   #$00
   ROL
   STA   __local_f3__0
   LDA   __local_f3__1
   EOR   __local_f3__0
   CLC
   ADC   g1
   STA   g1
   LDA   g0
   SEC
   SBC   __local_f3__1
   STA   __local_f3__4
   LDA   g0+1
   SBC   #$00
   STA   __local_f3__3
   LDA   g0+2
   SBC   #$00
   STA   __local_f3__2
   LDA   g0+3
   SBC   #$00
   STA   __local_f3__1
   LDA   #$07
   CMP   __local_f3__p0
   BNE   .cmp_true@24
   LDA   #$00
   BEQ   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   STA   __local_f3__0
   LDA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_f3__4
   STA   HARGS
   LDA   __local_f3__3
   STA   HARGS+1
   LDA   __local_f3__2
   STA   HARGS+2
   LDA   __local_f3__1
   STA   HARGS+3
   LDA   __local_f3__0
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JMP   mul32

f2:
   SUBROUTINE

   ; prologue: 6 arg bytes, 8 local bytes, 8 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
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
   LDA   $C1
   INY
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   $C3
   INY
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y
   LDA   $C5
   INY
   STA   (FP),Y
   LDA   $C6
   INY
   STA   (FP),Y
   LDA   $C7
   INY
   STA   (FP),Y

   LDY   #$0C
   LDA   (FP),Y
   EOR   #$FF
   STA   $83
   INY
   LDA   (FP),Y
   EOR   #$FF
   STA   $82
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   $81
   STA   $80
   LDY   #$0D
   LDA   (FP),Y
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   $C7
   STA   $C6
   LDY   #$0C
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   LDA   $C7
   STA   HARGS+6
   STA   HARGS+7
   LDA   $83
   STA   HARGS
   LDA   $82
   STA   HARGS+1
   LDA   $81
   STA   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+8
   STA   $C5
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   LDY   #$0E
   LDA   (FP),Y
   AND   #$07
   STA   HARGS+4
   LDA   $C5
   STA   HARGS
   JSR   asl32
   LDA   HARGS+5
   STA   $86
   LDA   HARGS+6
   STA   $85
   LDA   HARGS+7
   STA   $84
   LDA   HARGS+8
   STA   $83
   LDY   #$0B
   LDA   (FP),Y
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   $82
   STA   $81
   STA   $80
   LDY   #$0B
   LDA   (FP),Y
   CLC
   SBC   g0
   LDA   $82
   SBC   g0+1
   LDA   $81
   SBC   g0+2
   LDA   $80
   SBC   g0+3
   LDA   #$00
   ROL
   STA   $82
   LDA   #$00
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   STA   $81
   STA   $80
   LDA   $83
   CMP   $80
   BNE   .thread@1
   LDA   $84
   CMP   $81
   BNE   .thread@1
   LDA   $85
   BNE   .thread@1
   LDA   $86
   CMP   $82
   BEQ   .cmp_true@36
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   STA   $80
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0C
   LDA   (FP),Y
   ORA   $80
   CLC
   ADC   $83
   STA   $81
   LDA   $82
   ADC   #$00
   STA   $80
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f5
   STA   $C3
   LDA   #>f5
   STA   $C4
   LDY   #$0C
   LDA   (FP),Y
   STA   $C2
   LDA   #$00
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@0_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@39
   EOR   #$80
.jcmp_novf@39:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   LDA   #$FF
   SEC
   SBC   $81
   LDA   #$7F
   SBC   $80
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   STA   $86
   LDA   #$00
   BPL   .sx_done@42
   LDA   #$FF
.sx_done@42:
   STA   $85
   STA   $84
   LDA   g1
   STA   $81
   BMI   .sx_neg@43
   LDA   #$00
   BEQ   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   STA   $80
   LDA   $81
   SEC
   LDY   #$0C
   SBC   (FP),Y
   LDA   $80
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@45
   EOR   #$80
.cmp_novf@45:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $83
   LDA   #$00
   BPL   .sx_done@47
   LDA   #$FF
.sx_done@47:
   STA   $82
   STA   $81
   LDA   $C5
   LDY   #$0C
   AND   (FP),Y
   STA   $80
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDA   $86
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $85
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $C3
   STA   DPTR
   LDA   $C4
   STA   DPTR+1
   JSR   icall
   LDA   HARGS+8
   CLC
   ADC   $C1
   CLC
   ADC   $C2
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   CLC
   ADC   $C2
   STA   g1
   LDY   #$0C
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   LDA   $C7
   STA   HARGS+10
   LDA   $C6
   STA   HARGS+11

   ; epilogue
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   CLC
   LDA   FP
   ADC   #$10
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

f1:
   SUBROUTINE

   STA   __local_f1__pp
   STX   __local_f1__4
   LDA   g0
   AND   #$07
   TAX
   LDA   g1
   CPX   #$00
   BEQ   .shift_done@49
.shift_loop@48:
   ASL
   DEX
   BNE   .shift_loop@48
.shift_done@49:
   STA   __local_f1__3
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   BEQ   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   STA   __local_f1__2
   LDY   #$00
   LDA   (__local_f1__pp),Y
   CLC
   ADC   __local_f1__3
   STA   __local_f1__1
   INY
   LDA   (__local_f1__pp),Y
   ADC   __local_f1__2
   STA   (__local_f1__pp),Y
   LDA   __local_f1__1
   DEY
   STA   (__local_f1__pp),Y
   LDA   #$00
   STA   HARGS+8
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   STX   __local_f0__p0_1
   LDA   g0
   AND   #$07
   TAX
   LDA   __zpabi_f0__p1_0
   STA   __local_f0__5
   LDA   __zpabi_f0__p1_1
   STA   __local_f0__6
   CPX   #$00
   BEQ   .shift_done@53
.shift_loop@52:
   ASL   __local_f0__5
   ROL   __local_f0__6
   DEX
   BNE   .shift_loop@52
.shift_done@53:
   LDA   __zpabi_f0__p1_0
   ORA   __zpabi_f0__p1_1
   ORA   __zpabi_f0__p1_2
   ORA   __zpabi_f0__p1_3
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f0__4
   STA   __local_f0__3
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f0__4
   LDA   #$00
   STA   __local_f0__3
.or_end@1:
   LDA   __local_f0__p0_0
   SEC
   SBC   #$E8
   LDA   __local_f0__p0_1
   SBC   #$03
   LDA   #$00
   ROL
   STA   __local_f0__2
   LDA   #$00
   BPL   .sx_done@55
   LDA   #$FF
.sx_done@55:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __local_f0__p0_0
   STA   HARGS
   LDA   __local_f0__p0_1
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   STA   __zpabi_f4__p3_0
   LDA   HARGS+7
   STA   __zpabi_f4__p3_1
   LDA   __local_f0__5
   STA   __zpabi_f4__p0_0
   LDA   __local_f0__6
   STA   __zpabi_f4__p0_1
   LDA   __local_f0__2
   STA   __zpabi_f4__p2_0
   LDA   #$00
   STA   __zpabi_f4__p2_1
   LDA   __local_f0__1
   STA   __zpabi_f4__p2_2
   LDA   __local_f0__0
   STA   __zpabi_f4__p2_3
   LDX   __local_f0__3
   LDA   __local_f0__4
   JSR   f4
   LDA   #$00
   ROL
   STA   __local_f0__0
   LDA   __local_f0__p0_0
   EOR   __local_f0__0
   STA   __local_f0__3
   LDA   __local_f0__p0_1
   STA   __local_f0__2
   BMI   .sx_neg@56
   LDA   #$00
   BEQ   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g0
   CLC
   ADC   __local_f0__3
   STA   g0
   LDA   g0+1
   ADC   __local_f0__2
   STA   g0+1
   LDA   g0+2
   ADC   __local_f0__1
   STA   g0+2
   LDA   g0+3
   ADC   __local_f0__0
   STA   g0+3
   LDA   __zpabi_f0__p1_0
   STA   HARGS+8
   LDA   __zpabi_f0__p1_1
   STA   HARGS+9
   LDA   __zpabi_f0__p1_2
   STA   HARGS+10
   LDA   __zpabi_f0__p1_3
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$1D
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDX   #$00
   LDA   #$5B
   JSR   f0
   LDA   g1
   BMI   .sx_neg@58
   LDA   #$00
   BEQ   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
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
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__2
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__1
   EOR   g2+2
   STA   __local_main__1
   LDA   __local_main__0
   EOR   g2+3
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

g2:
   DC.L  $0000003E

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   LDA   #$80
   STA   g0
   LDA   #$9B
   STA   g1
   RTS
