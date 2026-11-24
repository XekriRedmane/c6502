SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g0	EQU	$28
g1	EQU	$29
__local_f3__0	EQU	$80
__local_f3__1	EQU	$81
__local_f3__2	EQU	$82
__local_f3__3	EQU	$83

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false carry=true
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3 indirect=false in_cycle=false
; def f4 params= locals= indirect=true in_cycle=false
; def f5 params= locals=__local_f5__0 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f4
; call f1 -> f2
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=2 at=auto def=true init=$20,$00 weight=25
; zp g0 size=1 at=auto def=true init=$3E weight=10
; zp g1 size=4 at=auto def=true init=$AA,$00,$00,$00 weight=28
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

   LDA   g2

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$08
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

f4:
   SUBROUTINE

   ; prologue: 1 arg bytes, 1 local bytes, 1 callee-saved bytes
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

   LDA   #$E8
   ORA   g2
   STA   $81
   LDA   g0
   ORA   #$E8
   AND   $81
   STA   $C0
   LDA   #<f5
   STA   $85
   LDA   #>f5
   STA   $86
   LDY   #$04
   LDA   (FP),Y
   SEC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   LDA   #$00
   SBC   g1+2
   LDA   #$00
   SBC   g1+3
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $84
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   $83
   STA   $82
   LDA   g0
   STA   $81
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   $80
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $84
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
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
   LDA   $85
   STA   DPTR
   LDA   $86
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C0
   EOR   $80
   CLC
   ADC   g2
   STA   g2
   BCC   .carry_inc@0
   INC   g2+1
.carry_inc@0:
   AND   #$07
   TAX
   LDA   #$FF
   STA   $82
   LDA   #$7F
   STA   $83
   CPX   #$00
   BEQ   .shift_done@6
.shift_loop@5:
   ASL   $82
   ROL   $83
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   g0
   STA   $81
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $80
   LDY   #$04
   LDA   (FP),Y
   SEC
   SBC   $81
   LDA   #$00
   SBC   $80
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   AND   #$07
   TAX
   BEQ   .shift_done@11
.shift_loop@10:
   LDA   $83
   ASL
   ROR   $83
   ROR   $82
   DEX
   BNE   .shift_loop@10
.shift_done@11:
   LDA   $83
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   HARGS+10
   STA   HARGS+11
   LDA   $82
   STA   HARGS+8
   LDA   $83
   STA   HARGS+9

   ; epilogue
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
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
   RTS

f3:
   SUBROUTINE

   LDA   g0
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f3__2
   LDA   g0
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f3__0
   LDA   g0
   CLC
   SBC   g2
   LDA   __local_f3__0
   SBC   g2+1
   LDA   #$00
   ROL
   STA   __local_f3__0
   LDA   g0
   SEC
   SBC   __local_f3__0
   LDA   __local_f3__2
   SBC   #$00
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f3__3
   BNE   .or_true@0
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_f3__2
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g1
   SEC
   SBC   __local_f3__3
   LDA   g1+1
   SBC   __local_f3__2
   LDA   g1+2
   SBC   __local_f3__1
   LDA   g1+3
   SBC   __local_f3__0
   BVC   .jcmp_novf@21
   EOR   #$80
.jcmp_novf@21:
   BMI   .or_true@0
   LDA   #$00
   STA   __local_f3__1
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f3__1
   LDA   #$00
.or_end@1:
   ORA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f2:
   SUBROUTINE

   ; prologue: 2 arg bytes, 10 local bytes, 10 callee-saved bytes
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
   LDA   $C6
   INY
   STA   (FP),Y
   LDA   $C7
   INY
   STA   (FP),Y
   LDA   $C8
   INY
   STA   (FP),Y
   LDA   $C9
   INY
   STA   (FP),Y

   LDA   g1
   STA   $C1
   LDA   g1+1
   STA   $C0
   LDA   g0
   STA   $80
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   HARGS+1
   LDA   g2
   ORA   #$01
   STA   HARGS+2
   LDA   g2+1
   STA   HARGS+3
   LDA   $80
   STA   HARGS
   JSR   udivmod16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   LDA   $81
   CLC
   SBC   #$FF
   LDA   $80
   SBC   #$00
   LDA   #$00
   ROL
   STA   $C9
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
   STA   $80
   LDA   $81
   CLC
   ADC   $C1
   STA   $81
   LDA   $80
   ADC   $C0
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
   LDA   #<f5
   STA   $C7
   LDA   #>f5
   STA   $C8
   LDA   $C9
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   $C6
   STA   $C5
   STA   $C4
   LDA   $C9
   STA   $C3
   LDA   #$00
   STA   $C2
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
   BPL   .loop@0_break
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C9
   LDY   #$01
   STA   (SSP),Y
   LDA   $C6
   INY
   STA   (SSP),Y
   LDA   $C5
   INY
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   LDA   g2
   INY
   STA   (SSP),Y
   LDA   g2+1
   INY
   STA   (SSP),Y
   LDA   $C7
   STA   DPTR
   LDA   $C8
   STA   DPTR+1
   JSR   icall
   STA   $81
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   $80
   LDA   $81
   CLC
   ADC   $C1
   STA   $81
   LDA   $80
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
   BNE   .loop@0_start
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $80
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $81
   LDA   g1
   AND   #$07
   TAX
   BEQ   .shift_done@32
.shift_loop@31:
   LDA   $81
   ASL
   ROR   $81
   ROR   $80
   DEX
   BNE   .shift_loop@31
.shift_done@32:
   LDA   $81
   ORA   $80
   BEQ   .cmp_end@33
   LDA   #$01
.cmp_end@33:

   ; epilogue
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
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
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
   PLA
   RTS

f1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes, 2 callee-saved bytes
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

   LDA   g0
   STA   $82
   BMI   .sx_neg@34
   LDA   #$00
   BEQ   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   STA   $81
   LDA   g0
   STA   $80
   BMI   .sx_neg@36
   LDA   #$00
   BEQ   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   STA   HARGS+3
   LDA   $82
   STA   HARGS
   STA   HARGS+2
   LDA   $81
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+4
   STA   $82
   LDA   HARGS+5
   STA   $81
   LDA   g2
   ORA   g2+1
   BEQ   .lnot_true@38
   LDA   #$00
   BEQ   .lnot_end@39
.lnot_true@38:
   LDA   #$01
.lnot_end@39:
   STA   $80
   LDA   $81
   BNE   .cmp_true@40
   LDA   $82
   CMP   $80
   BNE   .cmp_true@40
   LDA   #$00
   BEQ   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   STA   $C1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   g2
   STA   HARGS+2
   LDA   g2+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $83
   LDA   HARGS+5
   STA   $82
   LDA   g2+1
   STA   $80
   LDA   #$FF
   CLC
   SBC   $83
   LDA   $80
   SBC   $82
   LDA   #$00
   ROL
   STA   $C0
   LDA   #<f5
   STA   $82
   LDA   #>f5
   STA   $83
   LDA   g2
   AND   #$E8
   STA   $81
   LDA   g2+1
   AND   #$03
   STA   $80
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $81
   LDY   #$01
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$E8
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   LDA   $82
   STA   DPTR
   LDA   $83
   STA   DPTR+1
   JSR   icall
   STA   $81
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   BEQ   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   STA   $80
   LDA   $C0
   EOR   $81
   STA   $81
   LDA   #$00
   EOR   $80
   STA   $80
   LDA   g2
   CLC
   ADC   $81
   STA   g2
   LDA   g2+1
   ADC   $80
   STA   g2+1
   LDA   $C1
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
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f2
   LDA   g1
   AND   #$07
   TAX
   LDA   #$00
   STA   $80
   STA   $81
   CPX   #$00
   BEQ   .shift_done@46
.shift_loop@45:
   ASL   $80
   ROL   $81
   DEX
   BNE   .shift_loop@45
.shift_done@46:
   LDX   $81
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
   ADC   #$06
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
   TAX
   PLA
   RTS

f0:
   SUBROUTINE

   ; prologue: 0 arg bytes, 6 local bytes, 6 callee-saved bytes
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

   LDA   g1
   ORA   #$01
   STA   HARGS+4
   LDA   g1+1
   STA   HARGS+5
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+6
   STA   HARGS+2
   LDA   g1
   STA   HARGS
   LDA   g1+3
   STA   HARGS+7
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $C5
   LDA   #$00
   STA   $C4
   STA   $C3
   STA   $C2
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@1_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@47
   EOR   #$80
.jcmp_novf@47:
   BPL   .loop@1_break
   JSR   f1
   CLC
   ADC   $C1
   STA   $83
   TXA
   ADC   $C0
   STA   $82
   BMI   .sx_neg@48
   LDA   #$00
   BEQ   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   STA   $81
   AND   #$03
   STA   $80
   LDA   $C5
   CLC
   ADC   $83
   STA   $83
   LDA   $C4
   ADC   $82
   STA   $82
   LDA   $C3
   ADC   $81
   STA   $81
   LDA   $C2
   ADC   $80
   STA   $80
   LDA   $83
   STA   $C5
   LDA   $82
   STA   $C4
   LDA   $81
   STA   $C3
   LDA   $80
   AND   #$03
   STA   $C2
   INC   $C1
   BNE   .loop@1_start
   INC   $C0
   JMP   .loop@1_start
.loop@1_break:
   LDA   g1
   CLC
   ADC   $C5
   STA   g1
   LDA   g1+1
   ADC   $C4
   STA   g1+1
   LDA   g1+2
   ADC   $C3
   STA   g1+2
   LDA   g1+3
   ADC   $C2
   STA   g1+3
   LDA   #$00
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@2_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@50
   EOR   #$80
.jcmp_novf@50:
   BMI   .lb_skip@1
   JMP   .loop@2_break
.lb_skip@1:
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BNE   .or_true@2
   LDA   #$00
   STA   $80
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   $80
.or_end@3:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   JSR   f4
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   LDA   HARGS+10
   STA   $85
   LDA   HARGS+11
   STA   $84
   LDA   $C0
   BMI   .sx_neg@51
   LDA   #$00
   BEQ   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   STA   $81
   STA   $80
   LDA   $83
   CLC
   ADC   $C1
   STA   $83
   LDA   $82
   ADC   $C0
   STA   $82
   LDA   $85
   ADC   $81
   STA   $81
   LDA   $84
   ADC   $80
   STA   $80
   LDA   $C5
   CLC
   ADC   $83
   STA   $83
   LDA   $C4
   ADC   $82
   STA   $82
   LDA   $C3
   ADC   $81
   STA   $81
   LDA   $C2
   ADC   $80
   STA   $80
   LDA   $83
   STA   $C5
   LDA   $82
   STA   $C4
   LDA   $81
   STA   $C3
   LDA   $80
   AND   #$03
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@2_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@2_start
.loop@2_break:
   LDA   g2
   STA   $81
   LDA   g2+1
   STA   $80
   LDA   g1
   AND   $81
   STA   $87
   LDA   g1+1
   AND   $80
   STA   $86
   LDA   $81
   EOR   $C5
   STA   $83
   LDA   $80
   EOR   $C4
   STA   $82
   LDA   #$00
   EOR   $C3
   STA   $81
   LDA   #$00
   EOR   $C2
   STA   $80
   LDA   $87
   ORA   $83
   STA   $83
   LDA   $86
   ORA   $82
   STA   $82
   LDA   $80
   ORA   $81
   ORA   $82
   ORA   $83
   BEQ   .cmp_end@53
   LDA   #$01
.cmp_end@53:
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

   JSR   f0
   LDA   #$00
   ROL
   STA   $86
   LDA   g0
   STA   $83
   BMI   .sx_neg@54
   LDA   #$00
   BEQ   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   STA   $82
   STA   $81
   STA   $80
   LDA   $83
   EOR   g1
   STA   $83
   LDA   $82
   EOR   g1+1
   STA   $82
   LDA   $81
   EOR   g1+2
   STA   $85
   LDA   $80
   EOR   g1+3
   STA   $84
   LDA   g2
   STA   $81
   LDA   g2+1
   STA   $80
   LDA   $83
   EOR   $81
   STA   $83
   LDA   $82
   EOR   $80
   STA   $82
   LDA   $85
   STA   $81
   LDA   $84
   STA   $80
   LDA   $86
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g2+1
   LDA   #$3E
   STA   g0
   LDA   #$AA
   STA   g1
   LDA   #$20
   STA   g2
   RTS
