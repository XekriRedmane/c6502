SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__local_main__0	EQU	$80
__zpabi_f3__p0_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f3__p0_1	EQU	$81
__local_main__2	EQU	$82
__zpabi_f3__p0_2	EQU	$82
__local_main__3	EQU	$83
__zpabi_f3__p0_3	EQU	$83
__local_f3__1	EQU	$85
__local_main__5	EQU	$85

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0 indirect=false in_cycle=false carry=true
; def f1 params= locals= indirect=false in_cycle=false carry=true
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1 locals=__local_f3__0,__local_f3__1 indirect=false in_cycle=false param_regs=,,,,A
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f1 -> f2
; call f1 -> f3
; call f2 -> f3
; call f2 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$3D weight=14
; zp g0 size=2 at=auto def=true init=$98,$00 weight=11
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

f3:
   SUBROUTINE

   LDA   g0
   SEC
   SBC   #$02
   LDA   g0+1
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   SEC
   SBC   #$07
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__1
   LDA   g1
   EOR   __local_f3__1
   STA   __local_f3__1
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f2:
   SUBROUTINE

   ; prologue: 3 arg bytes, 1 local bytes, 1 callee-saved bytes
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

   LDY   #$04
   LDA   (FP),Y
   AND   #$07
   TAX
   LDA   g0
   CPX   #$00
   BEQ   .shift_done@6
.shift_loop@5:
   ASL
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   STA   $85
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $84
   LDA   $85
   ORA   #$01
   STA   HARGS+2
   LDA   $84
   STA   HARGS+3
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   STA   $85
   LDA   HARGS+7
   STA   $84
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __zpabi_f3__p0_2
   STA   __zpabi_f3__p0_3
   LDA   $85
   STA   __zpabi_f3__p0_0
   LDA   $84
   STA   __zpabi_f3__p0_1
   LDA   #$03
   JSR   f3
   LDA   HARGS+8
   STA   $84
   LDY   #$04
   LDA   (FP),Y
   EOR   $84
   STA   $C0
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   $84
   LDA   g1
   CLC
   ADC   $C0
   STA   g1
   LDA   $84
   CMP   #$00
   BNE   .cmp_true@13
   LDA   $C0
   LDY   #$04
   CMP   (FP),Y
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   $84
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
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   CLC
   ADC   $C0
   STA   $86
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   $84
   LDA   #$00
   SEC
   SBC   $86
   STA   $85
   LDA   $86
   SBC   $84
   STA   $84
   LDA   $85
   CLC
   LDY   #$05
   SBC   (FP),Y
   LDA   $84
   INY
   SBC   (FP),Y
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
   TAX
   PLA
   RTS

f1:
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

   LDA   #$E8
   ORA   g1
   STA   $84
   CLC
   SBC   g1
   LDA   #$00
   ROL
   STA   $C3
   LDA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   STA   $C2
   STA   $C1
   LDA   $C3
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   $C2
   STA   HARGS+2
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS+4
   LDA   #$7F
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $85
   LDA   g0
   SEC
   SBC   g1
   LDA   g0+1
   SBC   #$00
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   STA   $84
   SEC
   LDA   SSP
   SBC   #$03
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
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f2
   STA   $84
   LDA   #$E8
   EOR   $84
   STA   $C0
   CLC
   ADC   g1
   STA   g1
   LDA   g0
   STA   $88
   LDA   g0+1
   STA   $87
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   $86
   STA   $85
   LDA   $C3
   AND   #$07
   TAX
   LDA   #$01
   STA   $84
   CPX   #$00
   BEQ   .shift_done@24
.shift_loop@23:
   LSR   $84
   DEX
   BNE   .shift_loop@23
.shift_done@24:
   LDA   $88
   STA   __zpabi_f3__p0_0
   LDA   $87
   STA   __zpabi_f3__p0_1
   LDA   $86
   STA   __zpabi_f3__p0_2
   LDA   $85
   STA   __zpabi_f3__p0_3
   LDA   $84
   JSR   f3
   LDA   $C0
   EOR   HARGS+8
   CLC
   ADC   g0
   STA   g0
   BCC   .carry_inc@0
   INC   g0+1
.carry_inc@0:
   LDA   #$64
   SEC
   SBC   $C3
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   $C2
   LDA   #$00
   SBC   $C1
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   STA   HARGS
   LDA   g1
   STA   $84
   LDY   #$00
   STY   HARGS+1
   STA   HARGS+2
   STY   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   ORA   HARGS+4
   BEQ   .cmp_end@26
   LDA   #$01
.cmp_end@26:
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
   PLP
   RTS

f0:
   SUBROUTINE

   LDA   g1
   BEQ   .lnot_true@27
   LDA   #$00
   BEQ   .lnot_end@28
.lnot_true@27:
   LDA   #$01
.lnot_end@28:
   EOR   #$01
   CMP   #$01
   RTS

main:
   SUBROUTINE

   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__5
   LDA   g0+1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_main__1
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__5
   EOR   __local_main__3
   STA   __local_main__3
   LDA   #$00
   EOR   g0+1
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   HARGS+2
   LDA   #$00
   EOR   __local_main__0
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS
   LDA   __local_main__2
   STA   HARGS+1
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__3
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__2
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$98
   STA   g0
   LDA   #$3D
   STA   g1
   RTS
