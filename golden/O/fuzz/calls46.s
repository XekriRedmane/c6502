SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f2__0	EQU	$80
__local_f2__1	EQU	$81
__local_f2__2	EQU	$82

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$96 weight=25
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f2:
   SUBROUTINE

   ; prologue: 12 arg bytes, 0 local bytes
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
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   __local_f2__1
   INY
   LDA   (FP),Y
   STA   __local_f2__2
   CPX   #$00
   BEQ   .shift_done@1
.shift_loop@0:
   LDA   __local_f2__2
   ASL
   ROR   __local_f2__2
   ROR   __local_f2__1
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDA   #$FF
   LDY   #$03
   ORA   (FP),Y
   STA   __local_f2__0
   LDA   __local_f2__1
   EOR   __local_f2__0

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0E
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

   ; prologue: 0 arg bytes, 10 local bytes, 10 callee-saved bytes
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

   LDA   g0
   STA   $C9
   STA   $83
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   $82
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   $81
   STA   $80
   LDA   $83
   SEC
   SBC   g1
   LDA   $82
   SBC   g1+1
   LDA   $81
   SBC   g1+2
   LDA   $80
   SBC   g1+3
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $84
   LDA   g0
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $82
   LDA   g0
   STA   $81
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   $80
   LDA   $81
   SEC
   SBC   $83
   LDA   $80
   SBC   $82
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $80
   LDA   $84
   SEC
   SBC   $80
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   STA   $80
   AND   #$07
   STA   HARGS+4
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   JSR   asr32
   LDA   HARGS+5
   STA   $87
   LDA   HARGS+6
   STA   $86
   LDA   HARGS+7
   STA   $85
   LDA   HARGS+8
   STA   $84
   LDA   g0
   STA   $83
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $82
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $81
   STA   $80
   LDA   $87
   SEC
   SBC   $83
   LDA   $86
   SBC   $82
   LDA   $85
   SBC   $81
   LDA   $84
   SBC   $80
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   STA   $C3
   LDA   #<f2
   STA   $C7
   LDA   #>f2
   STA   $C8
   LDA   $C9
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $C6
   STA   $C5
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
   BVC   .jcmp_novf@20
   EOR   #$80
.jcmp_novf@20:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   g0
   BEQ   .and_false@0
   LDA   #$01
   STA   $83
   LDA   #$00
   STA   $82
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   $83
   STA   $82
.and_end@1:
   LDA   #$01
   ORA   $C3
   STA   $81
   LDA   $C2
   STA   $80
   SEC
   LDA   SSP
   SBC   #$0C
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
   LDA   $C9
   INY
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
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$02
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   $C7
   STA   DPTR
   LDA   $C8
   STA   DPTR+1
   JSR   icall
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
   LDA   g0
   STA   $81
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   $80
   LDA   $C3
   SEC
   SBC   $81
   LDA   $C2
   SBC   $80
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   PLA
   TAX
   PLA
   RTS

f0:
   SUBROUTINE

   ; prologue: 2 arg bytes, 9 local bytes, 9 callee-saved bytes
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

   LDA   g1
   STA   $C2
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   STA   HARGS+5
   LDA   g1+2
   STA   HARGS+2
   STA   HARGS+6
   LDA   g1
   STA   HARGS+4
   LDA   g1+3
   STA   HARGS+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   LDA   $83
   EOR   #$FF
   STA   $C8
   LDA   $82
   EOR   #$FF
   STA   $C7
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDA   $C8
   ORA   $C7
   BEQ   .and_false@2
   LDA   #$01
   STA   $81
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   $81
.and_end@3:
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   #$00
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
   LDA   #<f2
   STA   $C5
   LDA   #>f2
   STA   $C6
   LDA   $C7
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $C4
   STA   $C3
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
   BVC   .jcmp_novf@26
   EOR   #$80
.jcmp_novf@26:
   BMI   .lb_skip@3
   JMP   .loop@1_break
.lb_skip@3:
   LDA   g0
   STA   $80
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   HARGS+1
   LDA   $80
   STA   HARGS
   LDA   #$FF
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   $85
   LDA   HARGS+7
   STA   $84
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$0C
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
   LDA   $C8
   INY
   STA   (SSP),Y
   LDA   $C7
   INY
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   LDA   $C3
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
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
   LDA   $C5
   STA   DPTR
   LDA   $C6
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
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   HARGS+10
   STA   HARGS+11
   LDA   $81
   STA   HARGS+8
   LDA   $80
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
   RTS

main:
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

   LDA   #$05
   INY
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
   JSR   f0
   LDA   HARGS+8
   STA   $87
   LDA   HARGS+9
   STA   $86
   LDA   HARGS+10
   STA   $85
   LDA   HARGS+11
   STA   $84
   LDA   g0
   STA   $83
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
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
   STA   $81
   LDA   $80
   EOR   g1+3
   STA   $80
   LDA   $87
   EOR   $83
   STA   $C1
   LDA   $86
   EOR   $82
   STA   $C0
   LDA   $85
   EOR   $81
   STA   HARGS+2
   LDA   $84
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

g1:
   DC.L  $0000003A

__zp_init:
   SUBROUTINE

   LDA   #$96
   STA   g0
   RTS
