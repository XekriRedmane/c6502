SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> f2
; call f1 -> f2
; call f2 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$39 weight=7
; runtime hargs_size=32 hargs_need=24
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDA   g0
   STA   $80
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   HARGS+17
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   LDA   $80
   STA   HARGS+16

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

f1:
   SUBROUTINE

   ; prologue: 21 arg bytes, 1 local bytes, 1 callee-saved bytes
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

   LDA   g1
   STA   $81
   STA   $80
   SEC
   SBC   $80
   STA   $C0
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   $C0
   STA   HARGS
   LDY   #$10
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
   STA   $80
   LDA   HARGS+9
   STA   $83
   LDA   HARGS+10
   STA   $82
   LDA   HARGS+11
   STA   $81
   LDA   $83
   STA   $87
   LDA   $82
   STA   $86
   LDA   $81
   STA   $85
   LDA   $80
   SEC
   STA   $84
   LDA   $86
   STA   $86
   BCS   .carry_inc@0
   DEC   $85
.carry_inc@0:
   LDY   #$08
   LDA   (FP),Y
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   $80
   LDY   #$10
   LDA   (FP),Y
   LDY   #$08
   EOR   (FP),Y
   STA   $83
   LDY   #$11
   LDA   (FP),Y
   EOR   $80
   STA   $82
   SEC
   LDA   #$00
   LDY   #$04
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
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $84
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   $86
   INY
   STA   (SSP),Y
   LDA   $85
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
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f2
   LDA   g1
   LDX   g1+1

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$18
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

   ; prologue: 0 arg bytes, 9 local bytes, 9 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0A
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

   LDA   g0
   BEQ   .and_false@1
   LDA   #$01
   STA   $83
   LDA   #$00
   BEQ   .and_end@2
.and_false@1:
   LDA   #$00
   STA   $83
.and_end@2:
   ORA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   $81
   STA   $80
   LDA   g1
   SEC
   SBC   $83
   LDA   g1+1
   SBC   #$00
   LDA   g1+2
   SBC   $81
   LDA   g1+3
   SBC   $80
   LDA   #$00
   ROL
   STA   $C4
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   $C3
   STA   $C2
   STA   $C1
   STA   $C0
   LDA   g0
   STA   $85
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   $84
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   CMP   $C0
   BNE   .thread@0
   CMP   $C1
   BNE   .thread@0
   CMP   $C2
   BNE   .thread@0
   CMP   $C3
   BNE   .thread@0
   LDA   $84
   BNE   .thread@0
   LDA   $85
   CMP   $C4
   BEQ   .cmp_true@15
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   ORA   g0
   STA   $C8
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BNE   .or_true@3
   LDA   g0
   BNE   .or_true@3
   LDA   #$00
   STA   $84
   STA   $83
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   $84
   LDA   #$00
   STA   $83
.or_end@4:
   ORA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   STA   $82
   STA   $81
   LDA   $C0
   CMP   #$00
   BNE   .cmp_true@20
   LDA   #$00
   CMP   #$00
   BNE   .cmp_true@20
   LDA   $C4
   CMP   #$64
   BNE   .cmp_true@20
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   STA   $80
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $84
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
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$EA
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   LDA   #$64
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f2
   LDA   HARGS+16
   STA   $85
   LDA   HARGS+17
   STA   $84
   LDA   HARGS+18
   STA   $83
   LDA   HARGS+19
   STA   $82
   LDA   HARGS+20
   STA   $81
   LDA   HARGS+21
   STA   $80
   LDA   $C4
   EOR   $85
   STA   $C7
   LDA   #$00
   EOR   $84
   STA   $C6
   LDA   $C3
   EOR   $83
   STA   $C5
   LDA   $C2
   EOR   $82
   STA   $C4
   LDA   $C1
   EOR   $81
   STA   $C3
   LDA   $C0
   EOR   $80
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
   BVC   .jcmp_novf@23
   EOR   #$80
.jcmp_novf@23:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   $C7
   EOR   #$FF
   STA   $80
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C8
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$64
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$67
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   f1
   CLC
   ADC   $C1
   STA   $85
   TXA
   ADC   $C0
   STA   $84
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $83
   STA   $82
   STA   $81
   STA   $80
   LDA   $C7
   CLC
   ADC   $85
   STA   $85
   LDA   $C6
   ADC   $84
   STA   $84
   LDA   $C5
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
   AND   #$01
   STA   $80
   LDA   $85
   STA   $C7
   LDA   $84
   STA   $C6
   LDA   $83
   STA   $C5
   LDA   $82
   STA   $C4
   LDA   $81
   STA   $C3
   LDA   $80
   EOR   #$01
   SEC
   SBC   #$01
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   LDA   g1+2
   STA   $81
   LDA   g1+3
   STA   $80
   LDA   $83
   CLC
   ADC   $C7
   STA   $84
   LDA   $82
   ADC   $C6
   STA   $83
   LDA   $81
   ADC   $C5
   STA   $82
   LDA   $80
   ADC   $C4
   STA   $81
   LDA   $84
   STA   g1
   LDA   $83
   STA   g1+1
   LDA   $82
   STA   g1+2
   LDA   $81
   STA   g1+3
   LDA   #$02
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23

   ; epilogue
   INY
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
   CLC
   LDA   FP
   ADC   #$0B
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
   LDA   HARGS+16
   STA   $8B
   LDA   HARGS+17
   STA   $8A
   LDA   HARGS+18
   STA   $89
   LDA   HARGS+19
   STA   $88
   LDA   g0
   STA   $87
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   $86
   STA   $85
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
   LDA   $8B
   EOR   $83
   STA   $C1
   LDA   $8A
   EOR   $82
   STA   $C0
   LDA   $89
   EOR   $81
   STA   HARGS+2
   LDA   $88
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
   DC.L  $0000007A

__zp_init:
   SUBROUTINE

   LDA   #$39
   STA   g0
   RTS
