SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__zpabi_f1__p1_0	EQU	$80
__zpabi_f1__p1_1	EQU	$81
__zpabi_f1__p2_0	EQU	$82
__zpabi_f1__p2_1	EQU	$83
__zpabi_f1__p2_2	EQU	$84
__zpabi_f1__p2_3	EQU	$85

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params=__zpabi_f1__p0,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3 locals= indirect=false in_cycle=false param_regs=A,,,,,,
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f2
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$47 weight=23
; zp g0 size=2 at=auto def=true init=$8A,$00 weight=11
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f3:
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

   LDX   #$00
   LDA   #$07

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
   DEY
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

   LDY   #$09
   LDA   (FP),Y
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   $82
   STA   $81
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SBC   $82
   INY
   LDA   (FP),Y
   SBC   $81
   INY
   LDA   (FP),Y
   SBC   $80
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$02
   STA   HARGS
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   HARGS+1
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   HARGS+2
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   HARGS+3
   LDA   $80
   AND   #$07
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   $81
   LDA   HARGS+6
   AND   #$07
   STA   $80
   LDA   $81
   STA   $C3
   LDA   $80
   EOR   #$04
   SEC
   SBC   #$04
   STA   $C2
   LDA   #<f3
   STA   $C4
   LDA   #>f3
   STA   $C5
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
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   g0
   AND   g1
   STA   $83
   LDA   #$00
   EOR   #$04
   SEC
   SBC   #$04
   STA   $82
   LDA   g1
   AND   #$07
   TAX
   LDA   #$03
   STA   $80
   LDA   #$00
   STA   $81
   CPX   #$00
   BEQ   .shift_done@5
.shift_loop@4:
   ASL   $80
   ROL   $81
   DEX
   BNE   .shift_loop@4
.shift_done@5:
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
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
   STA   $81
   LDA   $C2
   ADC   $80
   AND   #$07
   STA   $80
   LDA   $81
   STA   $C3
   LDA   $80
   EOR   #$04
   SEC
   SBC   #$04
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
   ADC   $C3
   STA   g1

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

   LDA   __zpabi_f1__p1_0
   RTS

f0:
   SUBROUTINE

   ; prologue: 5 arg bytes, 7 local bytes, 7 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$08
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

   LDY   #$0B
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
   LDY   #$0B
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   LDA   HARGS+10
   STA   $81
   LDA   HARGS+11
   STA   $80
   LDA   #$00
   SEC
   SBC   $83
   LDA   #$00
   SBC   $82
   LDA   #$00
   SBC   $81
   LDA   #$00
   SBC   $80
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$00
   STA   $C6
   TYA
   EOR   #$04
   SEC
   SBC   #$04
   STA   $C5
   LDY   #$0A
   LDA   (FP),Y
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $82
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   #$00
   LDY   #$0E
   CMP   (FP),Y
   BNE   .cmp_true@9
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   $80
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   $81
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   EOR   $80
   STA   $C4
   INY
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $80
   LDA   #$00
   CMP   $82
   BNE   .cmp_true@13
   LDA   $80
   LDY   #$0A
   CMP   (FP),Y
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   $81
   LDY   #$00
   STA   $C3
   TYA
   EOR   #$04
   STA   $80
   SEC
   SBC   #$04
   STA   $C2
   LDA   #<f3
   STA   $C0
   LDA   #>f3
   STA   $C1
   LDA   g2
   ORA   #$01
   STA   HARGS+4
   LDA   g2+1
   STA   HARGS+5
   STA   HARGS+1
   LDA   g2+2
   STA   HARGS+6
   STA   HARGS+2
   LDA   g2
   STA   HARGS
   LDA   g2+3
   STA   HARGS+7
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   AND   #$07
   STA   $80
   LDA   $81
   STA   $83
   LDA   $80
   EOR   #$04
   SEC
   SBC   #$04
   STA   $82
   LDA   $C5
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   $81
   STA   $80
   LDA   $C6
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDA   $C5
   INY
   SBC   (FP),Y
   LDA   $81
   INY
   SBC   (FP),Y
   LDA   $80
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $C0
   STA   DPTR
   LDA   $C1
   STA   DPTR+1
   JSR   icall
   STA   $81
   STX   $80
   LDA   $C6
   EOR   $81
   STA   $83
   LDA   $C5
   EOR   $80
   STA   $82
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   $81
   STA   $80
   LDA   g2
   CLC
   ADC   $83
   STA   g2
   LDA   g2+1
   ADC   $82
   STA   g2+1
   LDA   g2+2
   ADC   $81
   STA   g2+2
   LDA   g2+3
   ADC   $80
   STA   g2+3
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
   BVC   .jcmp_novf@21
   EOR   #$80
.jcmp_novf@21:
   BMI   .lb_skip@3
   JMP   .loop@1_break
.lb_skip@3:
   LDA   $C2
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   $81
   STA   $80
   LDY   #$0E
   LDA   (FP),Y
   CMP   $80
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   $81
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   $C2
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   $C3
   BNE   .cmp_true@24
   LDA   #$00
   BEQ   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   STA   $80
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f2
   CLC
   ADC   $C1
   STA   $81
   LDA   #$00
   ADC   $C0
   STA   $80
   LDA   $C3
   CLC
   ADC   $81
   STA   $81
   LDA   $C2
   ADC   $80
   AND   #$07
   STA   $80
   LDA   $81
   STA   $C3
   LDA   $80
   EOR   #$04
   SEC
   SBC   #$04
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@2
   JMP   .loop@1_start
.lb_skip@2:
   INC   $C0
   JMP   .loop@1_start
.loop@1_break:
   LDA   #$E8

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
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$1B
   DEY
   STA   (SSP),Y
   LDA   #$18
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f0
   STA   $88
   LDA   g0
   STA   $82
   LDA   g0+1
   STA   $81
   LDA   $82
   EOR   g1
   STA   $87
   LDA   $81
   STA   $86
   LDA   #$00
   STA   $85
   STA   $84
   LDA   g2
   STA   $83
   LDA   g2+1
   STA   $82
   LDA   g2+2
   STA   $81
   LDA   g2+3
   STA   $80
   LDA   $87
   EOR   $83
   STA   $83
   LDA   $86
   EOR   $82
   STA   $82
   LDA   #$00
   EOR   $81
   STA   $81
   LDA   #$00
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

g2:
   DC.L  $000000A5

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$8A
   STA   g0
   LDA   #$47
   STA   g1
   RTS
