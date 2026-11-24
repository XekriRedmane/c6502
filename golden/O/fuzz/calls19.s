SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g2	EQU	$27
g1	EQU	$2B
__local_f2__0	EQU	$80

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals=__local_f2__0 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> rec
; call f1 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$82 weight=14
; zp g2 size=4 at=auto def=true init=$87,$00,$00,$00 weight=17
; zp g1 size=2 at=auto def=true init=$64,$00 weight=8
; runtime hargs_size=32 hargs_need=20
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

f2:
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

   LDY   #$05
   LDA   (FP),Y
   SEC
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   __local_f2__0
   LDY   #$05
   ORA   (FP),Y
   STA   __local_f2__0
   AND   #$07
   TAX
   BEQ   .shift_done@2
.shift_loop@1:
   LSR   __local_f2__0
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   LDA   __local_f2__0

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$08
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

f1:
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

   LDA   g2
   ORA   #$01
   STA   HARGS+4
   LDA   g2+1
   STA   HARGS+5
   LDA   g2+2
   STA   HARGS+6
   LDA   g2+3
   STA   HARGS+7
   LDY   #$04
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+12
   STA   $83
   LDA   HARGS+13
   STA   $82
   LDA   HARGS+14
   STA   $81
   LDA   HARGS+15
   STA   $80
   LDA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BEQ   .and_false@1
   LDA   g0
   BEQ   .and_false@1
   LDA   #$01
   STA   $81
   LDA   #$00
   STA   $80
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   STA   $81
   STA   $80
.and_end@2:
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
   LDA   g1
   STA   HARGS+8
   LDA   g1+1
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11

   ; epilogue
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
   RTS

f0:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes, 4 callee-saved bytes
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
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   $C3
   INY
   STA   (FP),Y

   LDA   g1
   STA   $81
   LDA   g1+1
   STA   $80
   LDA   g2
   CLC
   SBC   $81
   LDA   g2+1
   SBC   $80
   LDA   g2+2
   SBC   #$00
   LDA   g2+3
   SBC   #$00
   LDA   #$00
   ROL
   STA   $80
   LDA   #$01
   SEC
   SBC   $80
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   STA   $C2
   LDA   g1
   STA   $82
   LDA   g1+1
   STA   $80
   LDA   #$00
   CMP   g2+3
   BNE   .vthread@0
   CMP   g2+2
   BNE   .vthread@0
   CMP   g2+1
   BNE   .vthread@0
   LDA   #$01
   CMP   g2
   BEQ   .cmp_true@4
   LDA   #$00
.vthread@0:
.cmp_true@4:
   STA   $81
   LDA   $82
   SEC
   SBC   g2
   LDA   $80
   SBC   g2+1
   LDA   #$00
   SBC   g2+2
   LDA   #$00
   SBC   g2+3
   LDA   #$00
   ROL
   STA   $80
   LDA   $81
   SEC
   SBC   $80
   STA   $C3
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
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BPL   .loop@0_break
   LDA   $C3
   EOR   g2
   STA   $83
   LDA   #$00
   EOR   g2+1
   STA   $82
   LDA   #$00
   EOR   g2+2
   STA   $81
   LDA   #$00
   EOR   g2+3
   STA   $80
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   g0
   LDY   #$01
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
   JSR   f1
   LDA   HARGS+8
   CLC
   ADC   $C1
   CLC
   ADC   $C2
   STA   $C2
   INC   $C1
   BNE   .loop@0_start
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   $C2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   $80
   LDA   g1
   CLC
   ADC   $C2
   STA   g1
   LDA   g1+1
   ADC   $80
   STA   g1+1
   LDA   #<f2
   STA   $C0
   LDA   #>f2
   STA   $C1
   LDA   g0
   STA   $80
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   HARGS+2
   STA   HARGS+3
   LDA   g2
   ORA   #$01
   STA   HARGS+4
   LDA   g2+1
   STA   HARGS+5
   LDA   g2+2
   STA   HARGS+6
   LDA   g2+3
   STA   HARGS+7
   LDA   $80
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod32
   LDA   HARGS+12
   STA   $82
   LDA   HARGS+13
   STA   $81
   LDA   $C2
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   ORA   $C2
   BEQ   .cmp_true@15
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@15:
   LDA   #$01
.cmp_end@14:
   STA   $80
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$02
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$64
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
   LDA   $C0
   STA   DPTR
   LDA   $C1
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C2
   EOR   $80
   STA   $81
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
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
   LDA   g1
   ORA   #$01
   STA   HARGS+4
   LDA   g1+1
   STA   HARGS+5
   LDA   #$64
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   LDA   HARGS+11
   ORA   HARGS+10
   ORA   $82
   ORA   $83
   BEQ   .cmp_end@18
   LDA   #$01
.cmp_end@18:

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
   CLC
   LDA   FP
   ADC   #$06
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

   JSR   f0
   STA   $88
   LDA   g0
   STA   $82
   LDA   g1
   STA   $81
   LDA   g1+1
   STA   $80
   LDA   $82
   EOR   $81
   STA   $87
   LDA   #$00
   EOR   $80
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g2+1
   STA   g2+2
   STA   g2+3
   LDA   #$82
   STA   g0
   LDA   #$64
   STA   g1
   LDA   #$87
   STA   g2
   RTS
