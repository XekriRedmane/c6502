SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
__local_f3__0	EQU	$80
__zpabi_f0__p1	EQU	$80
__local_f3__1	EQU	$81
__zpabi_f0__p2	EQU	$81
__local_f3__2	EQU	$82
__zpabi_f0__p3_0	EQU	$82
__local_f3__3	EQU	$83
__zpabi_f0__p3_1	EQU	$83
__zpabi_f0__p3_2	EQU	$84
__zpabi_f0__p3_3	EQU	$85
__local_main__0	EQU	$86
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__5	EQU	$8B
__local_main__7	EQU	$8D
__local_f0__0	EQU	$90
__local_f0__p0_0	EQU	$93

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1,__zpabi_f0__p2,__zpabi_f0__p3_0,__zpabi_f0__p3_1,__zpabi_f0__p3_2,__zpabi_f0__p3_3 locals=__local_f0__0,__local_f0__1,__local_f0__p0_1,__local_f0__p0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$B8 weight=21
; zp g1 size=2 at=auto def=true init=$7A,$00 weight=13
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

rec:
   SUBROUTINE

   ; prologue: 4 arg bytes, 0 local bytes
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
   BMI   .jcmp_neg@0
   DEY
   ORA   (FP),Y
   BNE   .if_end@0
.jcmp_neg@0:
   LDY   #$05
   LDA   (FP),Y
   STA   $80
   INY
   LDA   (FP),Y
   LDX   #$00
   LDA   $80

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
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
   PLA
   TAX
   PLA
   RTS
.if_end@0:
   LDY   #$05
   LDA   (FP),Y
   ASL
   STA   $82
   INY
   LDA   (FP),Y
   ROL
   STA   $83
   LDA   $82
   CLC
   DEY
   ADC   (FP),Y
   STA   $81
   LDA   $83
   INY
   ADC   (FP),Y
   STA   $80
   LDA   $81
   CLC
   LDY   #$03
   ADC   (FP),Y
   STA   $81
   LDA   $80
   INY
   ADC   (FP),Y
   STA   $80
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   STA   $83
   INY
   LDA   (FP),Y
   SBC   #$00
   STA   $82
   LDA   $80
   AND   #$03
   STA   $80
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $83
   LDY   #$01
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
   JSR   rec
   STA   $80
   INC   $80
   BNE   .inc_done@0
   INX
.inc_done@0:
   LDA   $80

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
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
   PLA
   TAX
   PLA
   RTS

f3:
   SUBROUTINE

   ; prologue: 5 arg bytes, 0 local bytes
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

   LDA   g0
   STA   __local_f3__3
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f3__2
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   __local_f3__3
   LDY   #$04
   EOR   (FP),Y
   STA   __local_f3__3
   LDA   __local_f3__2
   INY
   EOR   (FP),Y
   STA   __local_f3__2
   LDA   __local_f3__1
   INY
   EOR   (FP),Y
   STA   __local_f3__1
   LDA   __local_f3__0
   INY
   EOR   (FP),Y
   STA   __local_f3__0
   LDA   __local_f3__3
   ORA   __local_f3__2
   ORA   __local_f3__1
   ORA   __local_f3__0
   BNE   .or_true@1
   LDY   #$03
   LDA   (FP),Y
   BEQ   .f3@ssa_block@4
   LDA   g0
   BNE   .or_true@5
.f3@ssa_block@4:
.or_true@1:
   LDY   #$03
   LDA   (FP),Y
   BNE   .or_true@5
   LDA   g0
   STA   __local_f3__1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f3__0
   LDA   __local_f3__1
   SEC
   SBC   g1
   LDA   __local_f3__0
   SBC   g1+1
   BCC   .or_true@5
   LDA   #$00
   LDX   #$00
   BEQ   .or_end@6
.or_true@5:
   LDA   #$01
   LDX   #$00
.or_end@6:

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$07
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
   PLA
   TAX
   PLA
   RTS

f2:
   SUBROUTINE

   ; prologue: 5 arg bytes, 6 local bytes, 6 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
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

   LDA   g0
   STA   $80
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   HARGS+3
   LDY   #$0B
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   $80
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+4
   STA   $C3
   LDA   HARGS+5
   STA   $C2
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $85
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $84
   LDA   g0
   STA   $83
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   $82
   LDA   g0
   STA   $81
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   $80
   LDA   $83
   SEC
   SBC   $81
   LDA   $82
   SBC   $80
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   CLC
   ADC   $85
   STA   $80
   BCC   .carry_inc@0
   INC   $84
.carry_inc@0:
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $84
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f3
   STA   $C4
   LDA   #>f3
   STA   $C5
   LDA   #$00
   STA   $C1
   DEY
   STY   $C0
.loop@0_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@14
   EOR   #$80
.jcmp_novf@14:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $80
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   LDA   $80
   STA   HARGS
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   $82
   LDA   g1
   STA   $81
   LDA   g1+1
   STA   $80
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $82
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   $C4
   STA   DPTR
   LDA   $C5
   STA   DPTR+1
   JSR   icall
   CLC
   ADC   $C1
   STA   $81
   TXA
   ADC   $C0
   STA   $80
   LDA   $C3
   CLC
   ADC   $81
   STA   $C3
   LDA   $C2
   ADC   $80
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   g0
   CLC
   ADC   $C3
   STA   g0
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $80
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   LDA   $80

   ; epilogue
   PHA
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
   CLC
   LDA   FP
   ADC   #$0D
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

f1:
   SUBROUTINE

   ; prologue: 1 arg bytes, 5 local bytes, 5 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
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

   LDA   g0
   STA   $C2
   LDA   #<f3
   STA   $C3
   LDA   #>f3
   STA   $C4
   LDA   #$00
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@1_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@15
   EOR   #$80
.jcmp_novf@15:
   BMI   .lb_skip@3
   JMP   .loop@1_break
.lb_skip@3:
   LDA   g0
   STA   $83
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   $82
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $81
   STA   $80
   LDA   #$FF
   SEC
   SBC   $83
   LDA   #$00
   SBC   $82
   LDA   #$00
   SBC   $81
   LDA   #$00
   SBC   $80
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $82
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   #$00
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
   CLC
   ADC   $C1
   CLC
   ADC   $C2
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@2
   JMP   .loop@1_start
.lb_skip@2:
   INC   $C0
   JMP   .loop@1_start
.loop@1_break:
   LDA   g0
   STA   $82
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   $81
   LDY   #$08
   LDA   (FP),Y
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   $80
   LDY   #$08
   LDA   (FP),Y
   CLC
   SBC   g1
   LDA   $80
   SBC   g1+1
   LDA   #$00
   ROL
   STA   $80
   ORA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDA   $82
   STA   HARGS
   LDA   $81
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   LDX   HARGS+5

   ; epilogue
   PHA
   TXA
   PHA
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
   PLA
   TAX
   PLA
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   LDA   g0
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_f0__0
   LDA   g0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__0
   STA   HARGS+3
   LDA   __local_f0__p0_0
   STA   HARGS
   STX   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   LDX   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$55
   STA   __zpabi_f0__p1
   LDA   #$ED
   STA   __zpabi_f0__p2
   LDA   #$23
   STA   __zpabi_f0__p3_0
   LDA   #$01
   STA   __zpabi_f0__p3_1
   LDA   #$00
   STA   __zpabi_f0__p3_2
   STA   __zpabi_f0__p3_3
   LDX   #$00
   LDA   #$1E
   JSR   f0
   STA   __local_main__7
   LDA   g0
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_main__2
   STA   __local_main__5
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__2
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__5
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__7
   EOR   __local_main__3
   STA   __local_main__3
   TXA
   EOR   __local_main__2
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   HARGS+2
   LDA   #$00
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
   STA   g1+1
   LDA   #$B8
   STA   g0
   LDA   #$7A
   STA   g1
   RTS
