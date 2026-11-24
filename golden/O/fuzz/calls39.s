SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false carry=true
; def f1 params= locals= indirect=false in_cycle=false carry=true
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params= locals= indirect=false in_cycle=false carry=true
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> f2
; call f1 -> f2
; call f1 -> f3
; call f2 -> f3
; call f3 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$06 weight=9
; zp g1 size=2 at=auto def=true init=$38,$00 weight=10
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

   ; prologue: 8 arg bytes, 0 local bytes
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
   LDY   #$04
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   rec
   SEC

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$0A
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
   PLP
   RTS

f2:
   SUBROUTINE

   ; prologue: 0 arg bytes, 5 local bytes, 5 callee-saved bytes
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
   STA   $C3
   STA   $C4
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
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   $C4
   SEC
   SBC   #$64
   STA   $86
   LDA   #$00
   SBC   #$00
   STA   $85
   LDA   #$00
   SBC   #$00
   STA   $84
   LDA   #$00
   SBC   #$00
   STA   $83
   LDA   #$00
   SBC   #$00
   STA   $82
   LDA   #$00
   SBC   #$00
   STA   $81
   LDA   #$00
   SBC   #$00
   STA   $80
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   $86
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
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   f3
   LDA   #$00
   ROL
   CLC
   ADC   $C1
   STA   $81
   LDA   #$00
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
   LDA   g1
   CLC
   ADC   $C3
   STA   g1
   LDA   g1+1
   ADC   $C2
   STA   g1+1
   LDA   g0
   BEQ   .and_false@3
   LDA   $C3
   ORA   $C2
   BEQ   .and_false@3
   LDA   #$01
   BNE   .and_end@4
.and_false@3:
   LDA   #$00
.and_end@4:
   CMP   $C3
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   AND   #$3F

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
   CLC
   LDA   FP
   ADC   #$07
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

   ; prologue: 2 arg bytes, 3 local bytes, 3 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$04
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

   LDY   #$06
   LDA   (FP),Y
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   $C2
   LDY   #$06
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   LDA   $C2
   STA   HARGS+3
   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   STA   $C0
   LDA   HARGS+5
   STA   $C1
   LDY   #$07
   LDA   (FP),Y
   AND   #$07
   STA   $80
   LDX   $80
   BEQ   .shift_done@7
.shift_loop@6:
   LDA   $C1
   ASL
   ROR   $C1
   ROR   $C0
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   #$FF
   SEC
   LDY   #$07
   SBC   (FP),Y
   STA   $87
   LDA   $C0
   STA   $85
   LDA   $C1
   STA   $86
   LDX   $80
   CPX   #$00
   BEQ   .shift_done@9
.shift_loop@8:
   ASL   $85
   ROL   $86
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   $86
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $84
   STA   $83
   STA   $82
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $87
   LDY   #$01
   STA   (SSP),Y
   LDA   $85
   INY
   STA   (SSP),Y
   LDA   $86
   INY
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
   JSR   f3
   BCC   .if_else@6
   LDA   $C0
   CLC
   ADC   #$03
   STA   $80
   JMP   .if_end@5
.if_else@6:
   LDA   $C0
   EOR   #$05
   STA   $80
.if_end@5:
   LDA   g0
   CLC
   ADC   $80
   STA   $80
   AND   #$3F
   STA   g0
   JSR   f2
   LDY   #$07
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   $82
   LDA   #$E8
   CLC
   LDY   #$06
   ADC   (FP),Y
   STA   $81
   LDA   #$03
   ADC   $C2
   STA   $80
   LDA   $82
   SEC
   SBC   $81
   LDA   #$00
   SBC   $80
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
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
   CLC
   LDA   FP
   ADC   #$07
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

f0:
   SUBROUTINE

   ; prologue: 3 arg bytes, 7 local bytes, 7 callee-saved bytes
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

   LDA   g1
   ORA   g1+1
   BEQ   .and_false@7
   LDA   g0
   BNE   .and_end@8
.and_false@7:
.and_end@8:
   LDA   #$02
   SEC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   LDA   #$00
   ROL
   ASL
   STA   $C5
   LDA   #$00
   STA   $C6
   ROL   $C6
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   $C3
   STA   $C2
   LDA   $C5
   STA   $C0
   LDY   #$0C
   LDA   (FP),Y
   EOR   #$FF
   STA   $81
   LDY   #$0A
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   STA   $80
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
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   f1
   LDA   #$00
   ROL
   STA   $80
   LDA   $C0
   EOR   $80
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
   BVC   .jcmp_novf@17
   EOR   #$80
.jcmp_novf@17:
   BPL   .loop@1_break
   JSR   f2
   CLC
   ADC   $C1
   STA   $83
   LDA   #$00
   ADC   $C0
   STA   $82
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $81
   STA   $80
   LDA   $C5
   CLC
   ADC   $83
   STA   $C5
   LDA   $C6
   ADC   $82
   STA   $C6
   LDA   $C3
   ADC   $81
   STA   $C3
   LDA   $C2
   ADC   $80
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
   ADC   $C6
   STA   g1+1
   LDA   $C4
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   HARGS+1
   LDY   #$0A
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   LDA   $C4
   STA   HARGS
   JSR   sdivmod16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   LDA   $81
   EOR   #$FF
   STA   $81
   LDA   $80
   EOR   #$FF
   ORA   $81
   BEQ   .cmp_end@22
   LDA   #$01
.cmp_end@22:
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
   CLC
   LDA   FP
   ADC   #$0C
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

   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$9C
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$1B
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   STA   $84
   LDA   g0
   STA   $82
   LDA   g1
   STA   $81
   LDA   g1+1
   STA   $80
   LDA   $82
   EOR   $81
   STA   $83
   LDA   #$00
   EOR   $80
   STA   $82
   LDA   $84
   EOR   $83
   STA   $83
   LDA   #$00
   EOR   $82
   STA   $82
   LDA   $83
   LDX   $82
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   LDA   #$06
   STA   g0
   LDA   #$38
   STA   g1
   RTS
