SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f3__0	EQU	$80
__local_f3__1	EQU	$81
__local_f3__2	EQU	$82

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false carry=true
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false carry=true
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call f1 -> f2
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=4 at=auto def=true init=$6B,$00,$00,$00 weight=21
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f3:
   SUBROUTINE

   ; prologue: 7 arg bytes, 0 local bytes
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

   LDY   #$09
   LDA   (FP),Y
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f3__0
   LDY   #$09
   LDA   (FP),Y
   LDY   #$07
   AND   (FP),Y
   STA   __local_f3__1
   LDA   __local_f3__0
   INY
   AND   (FP),Y
   ORA   __local_f3__1
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   STA   __local_f3__2
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   __local_f3__0
   ORA   __local_f3__2
   ORA   __local_f3__0
   BNE   .or_true@0
   LDA   g1+1
   STA   __local_f3__2
   LDA   g1+2
   STA   __local_f3__1
   LDA   g1+3
   STA   __local_f3__0
   LDA   g1
   ORA   __local_f3__2
   ORA   __local_f3__1
   ORA   __local_f3__0
   BEQ   .or_end@1
.or_true@0:
.or_end@1:
   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   LDA   #$00
   ROL
   AND   #$07
   TAX
   LDA   #$FF
   STA   __local_f3__0
   LDA   #$7F
   STA   __local_f3__1
   CPX   #$00
   BEQ   .shift_done@7
.shift_loop@6:
   ASL   __local_f3__0
   ROL   __local_f3__1
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   __local_f3__1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__0
   STA   HARGS+8
   LDA   __local_f3__1
   STA   HARGS+9

   ; epilogue
   CLC
   LDA   FP
   ADC   #$09
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

f2:
   SUBROUTINE

   ; prologue: 6 arg bytes, 6 local bytes, 6 callee-saved bytes
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
   STA   $C5
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   $80
   STA   $80
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f3
   STA   $C3
   LDA   #>f3
   STA   $C4
   LDA   #$01
   CLC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   LDA   #$00
   SBC   g0+2
   LDA   #$00
   SBC   g0+3
   LDA   #$00
   ROL
   STA   $C2
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   $C1
   STA   $C0
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   $83
   STA   HARGS
   LDA   $82
   STA   HARGS+1
   LDA   $81
   STA   HARGS+2
   LDA   $80
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $83
   LDA   HARGS+5
   STA   $82
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   LDA   g0
   EOR   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C2
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $C1
   INY
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
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
   LDA   $C5
   EOR   HARGS+8
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
   LDA   g0
   CLC
   ADC   $83
   STA   g0
   LDA   g0+1
   ADC   $82
   STA   g0+1
   LDA   g0+2
   ADC   $81
   STA   g0+2
   LDA   g0+3
   ADC   $80
   STA   g0+3
   LDA   $83
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   $80
   LDA   #$02
   SEC
   SBC   $83
   LDA   #$00
   SBC   $80
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   SEC
   SBC   #$F9
   LDA   #$00
   SBC   #$FF
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   CMP   #$01

   ; epilogue
   PHP
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
   CLC
   LDA   FP
   ADC   #$0E
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
   PLP
   RTS

f1:
   SUBROUTINE

   ; prologue: 8 arg bytes, 4 local bytes, 2 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
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

   SEC
   LDA   #$00
   SBC   g1
   STA   $83
   LDA   #$00
   SBC   g1+1
   STA   $82
   LDA   #$00
   SBC   g1+2
   STA   $81
   LDA   #$00
   SBC   g1+3
   BNE   .cmp_true@18
   LDA   $81
   BNE   .cmp_true@18
   LDA   $82
   CMP   #$7F
   BNE   .cmp_true@18
   LDA   $83
   CMP   #$FF
   BNE   .cmp_true@18
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   STA   $C1
   LDA   g1
   EOR   #$FF
   STA   $85
   LDA   g1+1
   EOR   #$FF
   STA   $84
   LDA   g1+2
   EOR   #$FF
   STA   $83
   LDA   g1+3
   EOR   #$FF
   STA   $82
   LDA   $C1
   CLC
   LDY   #$0B
   ADC   (FP),Y
   STA   $81
   LDA   #$00
   INY
   ADC   (FP),Y
   STA   $80
   LDA   $81
   SEC
   SBC   $85
   LDA   $80
   SBC   $84
   LDA   #$00
   SBC   $83
   LDA   #$00
   SBC   $82
   LDA   #$00
   ROL
   STA   $C0
   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $81
   LDA   $C1
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   CLC
   ADC   $82
   STA   $80
   BCC   .carry_inc@0
   INC   $81
.carry_inc@0:
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f3
   STA   $86
   LDA   #>f3
   STA   $87
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $85
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $84
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   $83
   STA   $82
   SEC
   LDA   #$00
   LDY   #$07
   SBC   (FP),Y
   STA   $81
   LDA   #$00
   INY
   SBC   (FP),Y
   STA   $80
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $85
   LDY   #$01
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDA   $86
   STA   DPTR
   LDA   $87
   STA   DPTR+1
   JSR   icall
   LDA   $C1
   EOR   HARGS+8
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$03
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   g0
   LDY   #$01
   STA   (SSP),Y
   LDA   g0+1
   INY
   STA   (SSP),Y
   LDA   g0+2
   INY
   STA   (SSP),Y
   LDA   g0+3
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f2
   LDA   #$00
   ROL
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $81
   LDY   #$03
   LDA   (FP),Y
   CLC
   ADC   $81
   STA   $83
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   $82
   STA   $81
   STA   $80
   LDA   g0
   CLC
   ADC   $83
   STA   g0
   LDA   g0+1
   ADC   $82
   STA   g0+1
   LDA   g0+2
   ADC   $81
   STA   g0+2
   LDA   g0+3
   ADC   $80
   STA   g0+3
   LDA   #$01

   ; epilogue
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   CLC
   LDA   FP
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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

   ; prologue: 10 arg bytes, 10 local bytes, 6 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
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

   LDY   #$16
   LDA   (FP),Y
   STA   $81
   LDY   #$0E
   LDA   (FP),Y
   SEC
   SBC   #$64
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $80
   ORA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   STA   HARGS+1
   LDA   $81
   STA   HARGS
   JSR   sdivmod16
   LDA   HARGS+6
   STA   $C5
   LDA   HARGS+7
   STA   $C4
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   $C3
   STA   $C2
   LDA   $C5
   LDY   #$07
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y
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
   BVC   .jcmp_novf@28
   EOR   #$80
.jcmp_novf@28:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDA   #$E9
   STA   HARGS+4
   LDA   #$03
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   STA   $85
   LDA   HARGS+9
   STA   $84
   LDA   HARGS+10
   STA   $83
   LDA   HARGS+11
   STA   $82
   CLC
   LDA   FP
   ADC   #$07
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $85
   LDY   #$01
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f1
   CLC
   ADC   $C1
   STA   $83
   LDA   #$00
   ADC   $C0
   STA   $82
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   $81
   STA   $80
   LDA   $C5
   CLC
   ADC   $83
   STA   $C5
   LDA   $C4
   ADC   $82
   STA   $C4
   LDA   $C3
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
   LDA   $C5
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   $C4
   DEY
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   LDA   #$64
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   $85
   LDA   HARGS+9
   STA   $84
   LDA   HARGS+10
   STA   $83
   LDA   HARGS+11
   STA   $82
   CLC
   LDA   FP
   ADC   #$09
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $85
   LDY   #$01
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f2
   BCS   .thread@0
   LDA   #$03
   BNE   .thread@0
   LDA   #$E8
   LDY   #$0D
   CMP   (FP),Y
   BEQ   .cmp_true@31
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   STA   $81
   LDA   #$01
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $80
   LDA   $81
   SEC
   SBC   $80
   STA   $81
   LDA   #$00
   SBC   #$00
   ORA   $81
   BEQ   .cmp_end@34
   LDA   #$01
.cmp_end@34:
   CMP   #$01

   ; epilogue
   PHP
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
   CLC
   LDA   FP
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLP
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes, 2 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
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

   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$7E
   DEY
   STA   (SSP),Y
   LDA   #$8D
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$CF
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$DA
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   STA   $88
   LDA   g0
   STA   $87
   LDA   g0+1
   STA   $86
   LDA   g0+2
   STA   $85
   LDA   g0+3
   STA   $84
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   LDA   g1+2
   STA   $81
   LDA   g1+3
   STA   $80
   LDA   $87
   EOR   $83
   STA   $83
   LDA   $86
   EOR   $82
   STA   $82
   LDA   $85
   EOR   $81
   STA   $81
   LDA   $84
   EOR   $80
   STA   $80
   LDA   $88
   EOR   $83
   STA   $C1
   LDA   #$00
   EOR   $82
   STA   $C0
   LDA   #$00
   EOR   $81
   STA   HARGS+2
   LDA   #$00
   EOR   $80
   STA   HARGS+3
   LDA   $C1
   STA   HARGS
   LDA   $C0
   STA   HARGS+1
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   $80
   LDA   HARGS+6
   STA   $81
   LDA   $C1
   EOR   $80
   STA   $80
   LDA   $C0
   EOR   $81
   TAX
   LDA   $80

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
   CLC
   LDA   FP
   ADC   #$04
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

g1:
   DC.L  $00000048

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   LDA   #$6B
   STA   g0
   RTS
