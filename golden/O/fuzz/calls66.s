SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_f1__p1_0	EQU	$80
__zpabi_f2__p0	EQU	$80
__zpabi_f1__p1_1	EQU	$81
__zpabi_f2__p1	EQU	$81
__zpabi_f1__p1_2	EQU	$82
__zpabi_f2__p2_0	EQU	$82
__zpabi_f1__p1_3	EQU	$83
__zpabi_f2__p2_1	EQU	$83
__zpabi_f1__p1_4	EQU	$84
__zpabi_f2__p2_2	EQU	$84
__zpabi_f1__p1_5	EQU	$85
__zpabi_f2__p2_3	EQU	$85
__zpabi_f2__p2_4	EQU	$86
__zpabi_f2__p2_5	EQU	$87
__local_f2__0	EQU	$88
__local_f1__3	EQU	$89
__local_f2__1	EQU	$89
__local_f2__p3_1	EQU	$8A
__local_f1__p0_0	EQU	$8B
__local_f2__p3_0	EQU	$8B
__local_f2__2	EQU	$8C
__local_f2__3	EQU	$8D

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p1_4,__zpabi_f1__p1_5 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__p0_1,__local_f1__p0_0 indirect=false in_cycle=false param_regs=A,X,,,,,, carry=true
; def f2 params=__zpabi_f2__p0,__zpabi_f2__p1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3,__zpabi_f2__p2_4,__zpabi_f2__p2_5,__zpabi_f2__p3_0,__zpabi_f2__p3_1 locals=__local_f2__0,__local_f2__1,__local_f2__p3_1,__local_f2__p3_0,__local_f2__2,__local_f2__3 indirect=false in_cycle=false param_regs=,,,,,,,,A,X carry=true
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> f2
; call f0 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@tail0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item g1 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=25
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

   STA   __local_f2__p3_0
   TXA
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f2__1
   AND   #$07
   STA   __local_f2__0
   LDA   g1
   AND   __local_f2__p3_0
   STA   __local_f2__p3_0
   TXA
   AND   g1+1
   STA   __local_f2__p3_1
   LDA   g1+2
   AND   __local_f2__1
   STA   __local_f2__1
   LDA   g1+3
   AND   __local_f2__0
   STA   __local_f2__0
   LDA   __local_f2__p3_0
   CLC
   ADC   g0
   STA   __local_f2__3
   LDA   __local_f2__p3_1
   ADC   g0+1
   STA   __local_f2__2
   LDA   __local_f2__1
   ADC   g0+2
   STA   __local_f2__p3_0
   LDA   __local_f2__0
   ADC   g0+3
   STA   __local_f2__p3_1
   LDA   #$00
   ADC   g0+4
   STA   __local_f2__1
   LDA   #$00
   ADC   g0+5
   AND   #$0F
   ORA   __local_f2__1
   ORA   __local_f2__p3_1
   ORA   __local_f2__p3_0
   ORA   __local_f2__2
   ORA   __local_f2__3
   JMP   f2@tail0

f2@tail0:
   SUBROUTINE

   BEQ   .cmp_end@3
   SEC
   RTS
.cmp_end@3:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0_0
   TXA
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f1__3
   LDA   __local_f1__p0_0
   STA   HARGS
   STX   HARGS+1
   LDA   __local_f1__3
   STA   HARGS+2
   STA   HARGS+3
   STA   HARGS+4
   AND   #$0F
   STA   HARGS+5
   LDA   g0
   ORA   #$01
   STA   HARGS+8
   LDA   g0+1
   STA   HARGS+9
   LDA   g0+2
   STA   HARGS+10
   LDA   g0+3
   STA   HARGS+11
   LDA   g0+4
   STA   HARGS+12
   LDA   g0+5
   STA   HARGS+13
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   BNE   .or_true@1
   LDA   __zpabi_f1__p1_0
   ORA   __zpabi_f1__p1_1
   ORA   __zpabi_f1__p1_2
   ORA   __zpabi_f1__p1_3
   ORA   __zpabi_f1__p1_4
   ORA   __zpabi_f1__p1_5
   BEQ   .or_end@2
.or_true@1:
.or_end@2:
   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   LDA   g0+4
   SBC   g0+4
   LDA   g0+5
   SBC   g0+5
   LDA   #$00
   ROL
   JMP   f2@tail0

f0:
   SUBROUTINE

   ; prologue: 8 arg bytes, 12 local bytes, 12 callee-saved bytes
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
   LDA   $CA
   INY
   STA   (FP),Y
   LDA   $CB
   INY
   STA   (FP),Y

   LDY   #$11
   LDA   (FP),Y
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   STA   $8C
   INY
   LDA   (FP),Y
   STA   $8D
   CPX   #$00
   BEQ   .shift_done@8
.shift_loop@7:
   ASL   $8C
   ROL   $8D
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   $89
   LDY   #$0F
   LDA   (FP),Y
   STA   $8B
   INY
   LDA   (FP),Y
   STA   $8A
   LDA   $89
   AND   #$07
   STA   $88
   LDA   $8B
   CLC
   INY
   SBC   (FP),Y
   LDA   $8A
   INY
   SBC   (FP),Y
   LDA   $89
   INY
   SBC   (FP),Y
   LDA   $88
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $88
   LDA   $8C
   SEC
   SBC   $88
   LDA   $8D
   SBC   #$00
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   STA   $C0
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   ORA   g0+4
   ORA   g0+5
   BEQ   .and_false@3
   LDA   #$01
   STA   $8E
   JMP   .vthread@0
.and_false@3:
   LDA   #$00
   STA   $8E
   LDA   #$01
.vthread@0:
   SEC
   LDY   #$11
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $8D
   LDA   g1
   SEC
   SBC   g1
   LDA   g1+1
   SBC   g1+1
   LDA   g1+2
   SBC   g1+2
   LDA   g1+3
   SBC   g1+3
   LDA   #$00
   ROL
   STA   $8C
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   $8B
   STA   $8A
   STA   $89
   STA   $88
   LDA   $8C
   STA   __zpabi_f2__p2_0
   LDA   #$00
   STA   __zpabi_f2__p2_1
   LDA   $8B
   STA   __zpabi_f2__p2_2
   LDA   $8A
   STA   __zpabi_f2__p2_3
   LDA   $89
   STA   __zpabi_f2__p2_4
   LDA   $88
   AND   #$0F
   STA   __zpabi_f2__p2_5
   LDA   g0
   STA   $88
   LDX   g0+1
   LDA   $8E
   STA   __zpabi_f2__p0
   LDA   $8D
   STA   __zpabi_f2__p1
   LDA   $88
   JSR   f2
   LDA   #$00
   ROL
   STA   $88
   BEQ   .if_else@6
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   STA   $8D
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $8C
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $8B
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $8A
   LDA   #$00
   ADC   #$00
   STA   $89
   LDA   #$00
   ADC   #$00
   STA   $88
   LDA   $8D
   STA   $CB
   LDA   $8C
   STA   $CA
   LDA   $8B
   STA   $C9
   LDA   $8A
   STA   $C8
   LDA   $89
   STA   $C7
   LDA   $88
   AND   #$0F
   STA   $C6
   JMP   .if_end@5
.if_else@6:
   LDY   #$11
   LDA   (FP),Y
   EOR   #$05
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $C8
   LDA   #$00
   STA   $C7
   STA   $C6
.if_end@5:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDA   $C0
   BNE   .or_true@7
   LDA   #$00
   STA   $C1
   STA   $C0
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   STA   $C1
   LDA   #$00
   STA   $C0
.or_end@8:
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $8D
   LDA   HARGS+5
   STA   $8C
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   $8B
   STA   $8A
   STA   $89
   STA   $88
   LDA   $8D
   STA   __zpabi_f1__p1_0
   LDA   $8C
   STA   __zpabi_f1__p1_1
   LDA   $8B
   STA   __zpabi_f1__p1_2
   LDA   $8A
   STA   __zpabi_f1__p1_3
   LDA   $89
   STA   __zpabi_f1__p1_4
   LDA   $88
   AND   #$0F
   STA   __zpabi_f1__p1_5
   LDX   $C0
   LDA   $C1
   JSR   f1
   LDA   #$00
   ROL
   STA   $88
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$0F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@11
   LDA   #$01
   STA   $8D
   LDA   #$00
   STA   $8C
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   STA   $8D
   LDY   #$00
   STY   $8C
.and_end@12:
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   $8B
   STA   $8A
   STA   $89
   STA   $88
   LDA   $8D
   STA   $C5
   LDA   #$00
   STA   $C4
   LDA   $8B
   STA   $C3
   STA   $C2
   STA   $C1
   AND   #$0F
   STA   $C0
   LDA   $CB
   ORA   #$01
   STA   HARGS+8
   LDA   $CA
   STA   HARGS+9
   LDA   $C9
   STA   HARGS+10
   LDA   $C8
   STA   HARGS+11
   LDA   $C7
   STA   HARGS+12
   LDA   $C6
   STA   HARGS+13
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   $C5
   AND   HARGS+24

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
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   CLC
   LDA   FP
   ADC   #$16
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
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$CD
   DEY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$A2
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$8A
   INY
   STA   (SSP),Y
   LDA   #$00
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

g0:
   DC.B  $3F
   DS.B  5

g1:
   DC.L  $0000004B
