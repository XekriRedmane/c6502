SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g2	EQU	$27
g0	EQU	$29
__zpabi_f4__p0	EQU	$80

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false carry=true
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals= indirect=true in_cycle=false
; def f4 params=__zpabi_f4__p0,__zpabi_f4__p1_0,__zpabi_f4__p1_1 locals= indirect=false in_cycle=false param_regs=,A,X
; def f5 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> rec
; call f1 -> f2
; call f1 -> rec
; call f3 -> f4
; call f3 -> rec
; call f5 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$17 weight=10
; zp g2 size=2 at=auto def=true init=$25,$00 weight=14
; zp g0 size=4 at=auto def=true init=$5A,$00,$00,$00 weight=17
; runtime hargs_size=32 hargs_need=22
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

f5:
   SUBROUTINE

   ; prologue: 2 arg bytes, 1 local bytes, 1 callee-saved bytes
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

   LDA   g0
   CLC
   ADC   g0
   STA   $84
   LDA   g0+1
   ADC   g0+1
   STA   $83
   LDA   g0+2
   ADC   g0+2
   STA   $82
   LDA   g0+3
   ADC   g0+3
   STA   $81
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .and_false@1
   LDA   #$01
   STA   $80
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   STA   $80
.and_end@2:
   LDA   $84
   STA   HARGS
   LDA   $83
   STA   HARGS+1
   LDA   $82
   STA   HARGS+2
   LDA   $81
   STA   HARGS+3
   LDA   $80
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   STA   $C0
   LDA   HARGS+6
   STA   $84
   LDA   HARGS+7
   STA   $83
   LDA   HARGS+8
   STA   $82
   LDA   $C0
   AND   #$07
   TAX
   LDA   g2
   STA   $80
   LDA   g2+1
   STA   $81
   CPX   #$00
   BEQ   .shift_done@2
.shift_loop@1:
   LDA   $81
   ASL
   ROR   $81
   ROR   $80
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   LDA   $80
   ORA   $81
   BEQ   .and_false@3
   LDA   $C0
   ORA   $84
   ORA   $83
   ORA   $82
   BEQ   .and_false@3
   LDA   #$01
   STA   $84
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   STA   $84
.and_end@4:
   LDY   #$04
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$04
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDA   g2
   EOR   #$FF
   STA   $81
   LDA   g2+1
   EOR   #$FF
   STA   $80
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   $80
   STA   $80
   LDY   #$04
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   g0
   STA   $81
   STA   HARGS
   LDA   g0+1
   STA   $80
   STA   HARGS+1
   LDA   $84
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
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
   JSR   rec
   STA   $80
   LDA   g1
   STA   $81
   LDA   $C0
   CLC
   ADC   $80
   ORA   $81

   ; epilogue
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$05
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

f4:
   SUBROUTINE

   LDX   g2+1
   LDA   g2
   RTS

f3:
   SUBROUTINE

   ; prologue: 4 arg bytes, 12 local bytes, 10 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
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
   STA   $84
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   $83
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $82
   STA   $81
   LDA   g0
   SEC
   SBC   $84
   LDA   g0+1
   SBC   $83
   LDA   g0+2
   SBC   $82
   LDA   g0+3
   SBC   $81
   BCC   .or_true@5
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .or_true@5
   LDA   #$00
   STA   $C1
   STA   $C0
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   STA   $C1
   LDA   #$00
   STA   $C0
.or_end@6:
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   $82
   STA   $81
   LDA   $C1
   EOR   g0
   STA   $86
   LDA   #$00
   EOR   g0+1
   STA   $85
   LDA   $82
   EOR   g0+2
   STA   $84
   LDA   $81
   EOR   g0+3
   STA   $83
   LDA   g1
   STA   $82
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   $81
   LDA   g2
   CLC
   ADC   $82
   STA   $82
   LDA   g2+1
   ADC   $81
   STA   $81
   LDA   $82
   ORA   #$01
   STA   $82
   LDA   $81
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   HARGS+6
   STA   HARGS+7
   LDA   $86
   STA   HARGS
   LDA   $85
   STA   HARGS+1
   LDA   $84
   STA   HARGS+2
   LDA   $83
   STA   HARGS+3
   LDA   $82
   STA   HARGS+4
   LDA   $81
   STA   HARGS+5
   JSR   udivmod32
   LDA   HARGS+12
   STA   $C5
   LDA   HARGS+13
   STA   $C4
   LDA   HARGS+14
   STA   $C3
   LDA   HARGS+15
   STA   $C2
   LDY   #$0F
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
   LDA   #$E9
   STA   HARGS+4
   LDA   #$03
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+12
   STA   $81
   LDX   HARGS+13
   LDY   #$0F
   LDA   (FP),Y
   STA   __zpabi_f4__p0
   LDA   $81
   JSR   f4
   STA   $82
   STX   $81
   LDA   $C1
   EOR   $82
   STA   $C9
   LDA   $C0
   EOR   $81
   STA   $C8
   LDA   g2
   CLC
   ADC   $C9
   STA   g2
   LDA   g2+1
   ADC   $C8
   STA   g2+1
   LDA   $C9
   LDY   #$0B
   STA   (FP),Y
   LDA   $C8
   INY
   STA   (FP),Y
   LDA   #<f5
   STA   $C6
   LDA   #>f5
   STA   $C7
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
   BVC   .jcmp_novf@15
   EOR   #$80
.jcmp_novf@15:
   BPL   .loop@0_break
   CLC
   LDA   FP
   ADC   #$0B
   STA   $81
   LDA   FP+1
   ADC   #$00
   STA   $82
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $81
   LDY   #$01
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $C6
   STA   DPTR
   LDA   $C7
   STA   DPTR+1
   JSR   icall
   STA   $82
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   $81
   LDA   $82
   CLC
   ADC   $C1
   STA   $84
   LDA   $81
   ADC   $C0
   STA   $83
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $82
   STA   $81
   LDA   $C5
   CLC
   ADC   $84
   STA   $C5
   LDA   $C4
   ADC   $83
   STA   $C4
   LDA   $C3
   ADC   $82
   STA   $C3
   LDA   $C2
   ADC   $81
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$0B
   LDA   (FP),Y
   STA   $84
   INY
   LDA   (FP),Y
   STA   $83
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   $82
   STA   $81
   LDA   $C5
   CLC
   ADC   $84
   STA   $84
   LDA   $C4
   ADC   $83
   STA   $83
   LDA   $C3
   ADC   $82
   STA   $82
   LDA   $C2
   ADC   $81
   STA   $81
   LDA   g0
   CLC
   ADC   $84
   STA   g0
   LDA   g0+1
   ADC   $83
   STA   g0+1
   LDA   g0+2
   ADC   $82
   STA   g0+2
   LDA   g0+3
   ADC   $81
   STA   g0+3
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   #$07
   SEC
   SBC   $C9
   STA   $88
   LDA   #$00
   SBC   $C8
   STA   $87
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   $86
   STA   $85
   LDA   g2
   STA   $84
   LDA   g2+1
   STA   $83
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $82
   STA   $81
   LDY   #$0F
   LDA   (FP),Y
   SEC
   SBC   $84
   STA   $84
   INY
   LDA   (FP),Y
   SBC   $83
   STA   $83
   INY
   LDA   (FP),Y
   SBC   $82
   STA   $82
   INY
   LDA   (FP),Y
   SBC   $81
   STA   $81
   LDA   $84
   SEC
   SBC   $88
   LDA   $83
   SBC   $87
   LDA   $82
   SBC   $86
   LDA   $81
   SBC   $85
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   LDX   #$00

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
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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

   ; prologue: 1 arg bytes, 7 local bytes, 5 callee-saved bytes
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

   LDA   g1
   STA   $81
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   $80
   LDA   $81
   SEC
   SBC   #$64
   LDA   $80
   SBC   #$00
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   EOR   #$01
   STA   $C2
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$07
   STA   (FP),Y
   LDA   #<f5
   STA   $C3
   LDA   #>f5
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
   BVC   .jcmp_novf@32
   EOR   #$80
.jcmp_novf@32:
   BPL   .loop@1_break
   CLC
   LDA   FP
   ADC   #$06
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
   BNE   .loop@1_start
   INC   $C0
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   $C2
   STA   $81
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   $80
   LDA   g2
   CLC
   ADC   $81
   STA   g2
   LDA   g2+1
   ADC   $80
   STA   g2+1
   LDY   #$0A
   LDA   (FP),Y
   EOR   (FP),Y
   SEC
   SBC   #$17
   LDA   #$00
   SBC   #$FC
   BVC   .cmp_novf@35
   EOR   #$80
.cmp_novf@35:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $80
   LDA   #$00
   BPL   .sx_done@37
   LDA   #$FF
.sx_done@37:
   STA   HARGS+10
   STA   HARGS+11
   LDA   $80
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9

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
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$08
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

   ; prologue: 6 arg bytes, 12 local bytes, 10 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
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

   LDA   #$64
   SEC
   SBC   g2
   LDA   #$00
   SBC   g2+1
   BVC   .cmp_novf@38
   EOR   #$80
.cmp_novf@38:
   ASL
   LDA   #$00
   ROL
   STA   $80
   LDY   #$14
   LDA   (FP),Y
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   $C9
   LDA   $80
   LDY   #$14
   AND   (FP),Y
   STA   $81
   LDA   #$00
   AND   $C9
   STA   $80
   LDA   g0
   STA   $C5
   LDA   g0+1
   STA   $C4
   LDA   g0+2
   STA   $C3
   LDA   g0+3
   STA   $C2
   LDA   g0
   STA   $C8
   LDA   $81
   LDY   #$0B
   STA   (FP),Y
   LDA   $80
   INY
   STA   (FP),Y
   LDA   #<f5
   STA   $C6
   LDA   #>f5
   STA   $C7
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
   BVC   .jcmp_novf@41
   EOR   #$80
.jcmp_novf@41:
   BPL   .loop@2_break
   CLC
   LDA   FP
   ADC   #$0B
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
   LDA   $C6
   STA   DPTR
   LDA   $C7
   STA   DPTR+1
   JSR   icall
   STA   $81
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   BEQ   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   STA   $80
   LDA   $81
   CLC
   ADC   $C1
   STA   $83
   LDA   $80
   ADC   $C0
   STA   $82
   BMI   .sx_neg@44
   LDA   #$00
   BEQ   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
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
   BEQ   .lb_skip@1
   JMP   .loop@2_start
.lb_skip@1:
   INC   $C0
   JMP   .loop@2_start
.loop@2_break:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C8
   LDY   #$01
   STA   (SSP),Y
   JSR   f2
   LDA   g1
   STA   $81
   BMI   .sx_neg@46
   LDA   #$00
   BEQ   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   STA   $80
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
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
   JSR   rec
   LDA   $C9
   LDY   #$14
   ORA   (FP),Y
   BEQ   .cmp_end@48
   LDA   #$01
.cmp_end@48:
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
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLP
   RTS

f0:
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

   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   DEY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   g1

   ; epilogue
   PHA
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
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$02
   DEY
   STA   (SSP),Y
   LDA   #$1D
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
   LDA   #$DA
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$0D
   INY
   STA   (SSP),Y
   JSR   f0
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
   BMI   .sx_neg@49
   LDA   #$00
   BEQ   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   STA   $82
   STA   $81
   STA   $80
   LDA   $87
   EOR   $83
   STA   $87
   LDA   $86
   EOR   $82
   STA   $86
   LDA   $85
   EOR   $81
   STA   $85
   LDA   $84
   EOR   $80
   STA   $84
   LDA   g2
   STA   $83
   LDA   g2+1
   STA   $82
   BMI   .sx_neg@51
   LDA   #$00
   BEQ   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   STA   $81
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   STA   g2+1
   LDA   #$5A
   STA   g0
   LDA   #$17
   STA   g1
   LDA   #$25
   STA   g2
   RTS
