SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> f3
; call f0 -> rec
; call f1 -> f2
; call f2 -> f3
; call f2 -> rec
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
; zp g0 size=1 at=auto def=true init=$76 weight=31
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

f3:
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
   CLC
   SBC   g0
   LDA   #$00
   ROL
   STA   $C4
   LDA   g0
   BNE   .or_true@1
   LDA   $C4
   BEQ   .vthread@0
.or_true@1:
   LDA   $C4
.vthread@0:
   SEC
   SBC   g0
   LDA   #$00
   ROL
   STA   $80
   SEC
   LDA   #$00
   SBC   $80
   STA   $C3
   LDA   #$00
   SBC   #$00
   STA   $C2
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   $C1
   STA   $C0
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
   LDA   #$9C
   INY
   STA   (SSP),Y
   LDA   #$63
   INY
   STA   (SSP),Y
   JSR   rec
   STA   $81
   STX   $80
   LDA   $C3
   ORA   $C2
   ORA   $C1
   ORA   $C0
   BNE   .or_true@3
   LDA   $C4
   BNE   .or_true@3
   LDA   #$00
   STA   $83
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   $83
.or_end@4:
   LDA   $C3
   CLC
   ADC   $81
   STA   $81
   LDA   $C2
   ADC   $80
   STA   $80
   LDA   $83
   CLC
   SBC   $81
   LDA   #$00
   SBC   $80
   LDA   #$00
   ROL

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

f2:
   SUBROUTINE

   ; prologue: 13 arg bytes, 4 local bytes, 4 callee-saved bytes
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

   LDY   #$0E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   LDA   #$01
   STA   $83
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   STA   $83
.and_end@6:
   LDY   #$0A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDY   #$08
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$01
   STA   $81
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   STA   $81
.and_end@8:
   LDA   $83
   SEC
   SBC   $81
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   $C3
   LDY   #$12
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$12
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0E
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   STA   $80
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   $80
   STA   $80
   LDY   #$12
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   STA   $C2
   STA   $C1
   DEY
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
   BPL   .loop@0_break
   JSR   f3
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
   BNE   .loop@0_start
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   g0
   CLC
   ADC   $C3
   STA   g0
   LDY   #$07
   LDA   (FP),Y
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   $80
   LDY   #$07
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   LDA   $80
   STA   HARGS+3
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
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
   LDY   #$07
   LDA   (FP),Y
   AND   #$07
   STA   HARGS+4
   LDY   #$0E
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
   JSR   asl32
   LDA   HARGS+5
   STA   HARGS
   LDA   HARGS+6
   STA   HARGS+1
   LDA   HARGS+7
   STA   HARGS+2
   LDA   HARGS+8
   STA   HARGS+3
   LDA   $C3
   AND   #$07
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDX   HARGS+6

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
   CLC
   LDA   FP
   ADC   #$13
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

f1:
   SUBROUTINE

   ; prologue: 2 arg bytes, 9 local bytes, 7 callee-saved bytes
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

   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   $C3
   STA   $C2
   LDA   g0
   CLC
   SBC   g0
   LDA   #$00
   ROL
   STA   $C6
   LDA   #$00
   STA   $80
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   HARGS+2
   STA   HARGS+3
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $C1
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $C0
   LDA   $C6
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$FF
   STY   HARGS+4
   STA   HARGS+5
   INY
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   STA   $80
   LDA   $C1
   CLC
   ADC   $81
   STA   $81
   LDA   $C0
   ADC   $80
   STA   $80
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   STA   $C5
   STA   $C4
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@1_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@10
   EOR   #$80
.jcmp_novf@10:
   BMI   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDA   $C6
   ORA   #$03
   STA   $8A
   LDA   g0
   STA   $89
   STA   $82
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   $81
   STA   $80
   LDA   $C2
   CMP   $80
   BNE   .cmp_true@13
   LDA   $C3
   CMP   $81
   BNE   .cmp_true@13
   LDA   $C4
   BNE   .cmp_true@13
   LDA   $C5
   CMP   $82
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   $88
   LDA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   $87
   STA   $86
   LDA   $C5
   SEC
   SBC   $C5
   STA   $85
   LDA   $C4
   SBC   $C4
   STA   $84
   LDA   $C3
   SBC   $C3
   STA   $83
   LDA   $C2
   SBC   $C2
   STA   $82
   CLC
   LDA   FP
   ADC   #$08
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$0D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $8A
   LDY   #$01
   STA   (SSP),Y
   LDA   $89
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
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
   CLC
   ADC   $C1
   STA   $81
   TXA
   ADC   $C0
   STA   $80
   LDA   $C5
   CLC
   ADC   $81
   STA   $C5
   LDA   $C4
   ADC   $80
   STA   $C4
   BCC   .carry_inc@0
   INC   $C3
   BNE   .carry_inc@0
   INC   $C2
.carry_inc@0:
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@1_start
.loop@1_break:
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
   INY
   LDA   (FP),Y
   STA   $C6
   CLC
   LDA   FP
   ADC   #$0D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0A
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

   ; prologue: 5 arg bytes, 5 local bytes, 3 callee-saved bytes
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

   LDY   #$08
   LDA   (FP),Y
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $80
   LDY   #$0C
   LDA   (FP),Y
   CMP   $80
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   STA   $C0
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   $C2
   STA   $C1
   JSR   f3
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $C0
   LDA   #$00
   STA   $80
   LDA   g0
   CLC
   ADC   $C0
   STA   g0
   LDA   $C0
   LDY   #$04
   STA   (FP),Y
   LDA   $80
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$04
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
   JSR   f1
   STA   $80
   LDY   #$04
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   LDA   $C0
   EOR   $80
   CLC
   ADC   $81
   STA   $80
   CLC
   ADC   g0
   STA   g0
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$0A
   LDA   (FP),Y

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
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$06
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

   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$FE
   LDY   #$01
   STA   (SSP),Y
   LDA   #$96
   INY
   STA   (SSP),Y
   LDA   #$2B
   INY
   STA   (SSP),Y
   LDA   #$08
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   JSR   f0
   EOR   g0
   LDX   #$00
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$76
   STA   g0
   RTS
