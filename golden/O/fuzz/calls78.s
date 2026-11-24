SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f3__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f1__p0_1	EQU	$81
__zpabi_f1__p0_2	EQU	$82
__zpabi_f1__p0_3	EQU	$83
__zpabi_f1__p1	EQU	$84
__zpabi_f1__p2_0	EQU	$85
__zpabi_f1__p2_1	EQU	$86
__zpabi_f1__p2_2	EQU	$87
__zpabi_f1__p2_3	EQU	$88
__local_f1__1	EQU	$8A

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3 locals=__local_f1__0,__local_f1__1 indirect=false in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals=__local_f3__0 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$AA,$00 weight=14
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f3:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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

   LDA   g0
   ORA   g0+1
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   STA   __local_f3__0
   LDA   g0
   ORA   g0+1
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   STA   HARGS+1
   LDA   __local_f3__0
   STA   HARGS
   JSR   mul8
   LDA   HARGS+2
   STA   __local_f3__0
   LDY   #$04
   LDA   (FP),Y
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_f3__0
   ORA   #$01
   STA   HARGS+4
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$04
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

f2:
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

   LDA   g0
   CLC
   ADC   g0
   STA   HARGS
   LDA   g0+1
   ADC   g0+1
   STA   HARGS+1
   LDA   g0
   ORA   #$01
   STA   HARGS+2
   LDA   g0+1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   $C3
   LDA   HARGS+7
   STA   $C2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   $C1
   STA   $C0
   LDA   #<f3
   STA   $80
   LDA   #>f3
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C3
   LDY   #$01
   STA   (SSP),Y
   LDA   $C2
   INY
   STA   (SSP),Y
   LDA   $80
   STA   DPTR
   LDA   $81
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C3
   EOR   $80
   STA   $87
   LDA   $C2
   STA   $86
   LDA   $C1
   STA   $85
   LDA   $C0
   STA   $84
   LDA   g0
   CLC
   ADC   $87
   STA   g0
   LDA   g0+1
   ADC   $86
   STA   g0+1
   STA   $82
   LDA   g0
   STA   $83
   LDA   g0+1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   $81
   STA   $80
   LDA   $83
   SEC
   SBC   $87
   LDA   $82
   SBC   $86
   LDA   $81
   SBC   $85
   LDA   $80
   SBC   $84
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   STA   $80
   LDA   $87
   AND   #$07
   TAX
   LDA   #$00
   STA   $81
   CPX   #$00
   BEQ   .shift_done@12
.shift_loop@11:
   LDA   $81
   ASL
   ROR   $81
   ROR   $80
   DEX
   BNE   .shift_loop@11
.shift_done@12:
   LDA   $81
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   HARGS+10
   STA   HARGS+11
   LDA   $80
   STA   HARGS+8
   LDA   $81
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
   RTS

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p1
   CMP   __zpabi_f1__p1
   BCS   .or_true@0
   CMP   #$01
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f1__1
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f1__1
   LDA   #$00
.or_end@1:
   ORA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f0:
   SUBROUTINE

   ; prologue: 0 arg bytes, 1 local bytes, 1 callee-saved bytes
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
   ORA   g0+1
   BNE   .or_true@2
   LDA   #$00
   STA   $8D
   STA   $8C
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   $8D
   LDA   #$00
   STA   $8C
.or_end@3:
   ORA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   $8B
   STA   $8A
   LDA   g0
   AND   #$07
   TAX
   LDA   $8D
   CPX   #$00
   BEQ   .shift_done@20
.shift_loop@19:
   LSR
   DEX
   BNE   .shift_loop@19
.shift_done@20:
   LDX   #$FF
   SEC
.div_loop@21:
   INX
   SBC   #$65
   BCS   .div_loop@21
   ADC   #$65
   STA   $C0
   LDA   $8D
   CLC
   SBC   $C0
   LDA   #$00
   SBC   #$00
   LDA   $8B
   SBC   #$00
   LDA   $8A
   SBC   #$00
   LDA   #$00
   ROL
   STA   $8A
   LDA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_f1__p0_2
   STA   __zpabi_f1__p0_3
   LDA   g0
   STA   $89
   ORA   $C0
   STA   __zpabi_f1__p1
   LDA   $8A
   STA   __zpabi_f1__p0_0
   LDA   #$07
   STA   __zpabi_f1__p2_0
   LDA   #$00
   STA   __zpabi_f1__p0_1
   STA   __zpabi_f1__p2_1
   STA   __zpabi_f1__p2_2
   STA   __zpabi_f1__p2_3
   JSR   f1
   JSR   f2
   LDX   #$00
   LDA   $C0

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$03
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
   STA   $C1
   STX   $C0
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $81
   STA   $80
   LDA   $C1
   EOR   $83
   STA   HARGS
   TXA
   EOR   $82
   STA   HARGS+1
   LDA   #$00
   EOR   $81
   STA   HARGS+2
   LDA   #$00
   EOR   $80
   STA   HARGS+3
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   $83
   LDA   HARGS+6
   STA   $82
   LDA   $C1
   EOR   g0
   STA   $80
   LDA   $C0
   EOR   g0+1
   STA   $81
   LDA   $80
   EOR   $83
   STA   $80
   LDA   $81
   EOR   $82
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
   LDA   #$AA
   STA   g0
   RTS
