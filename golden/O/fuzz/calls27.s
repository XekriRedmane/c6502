SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g0	EQU	$27
__local_f5__0	EQU	$80
__zpabi_f1__p1_0	EQU	$80
__zpabi_f3__p1_0	EQU	$80
__local_f5__1	EQU	$81
__zpabi_f1__p1_1	EQU	$81
__zpabi_f3__p1_1	EQU	$81
__zpabi_f1__p1_2	EQU	$82
__zpabi_f3__p1_2	EQU	$82
__zpabi_f1__p1_3	EQU	$83
__zpabi_f3__p1_3	EQU	$83
__zpabi_f1__p1_4	EQU	$84
__local_f2__1	EQU	$85
__zpabi_f1__p2_0	EQU	$85
__local_f2__l0_1	EQU	$86
__zpabi_f1__p2_1	EQU	$86
__local_f2__l0_0	EQU	$87
__zpabi_f1__p2_2	EQU	$87
__zpabi_f1__p2_3	EQU	$88
__zpabi_f1__p2_4	EQU	$89
__local_f2__it_1	EQU	$8A
__local_f2__it_0	EQU	$8B
__local_f3__0	EQU	$8E
__local_f3__1	EQU	$8F
__local_f3__2	EQU	$90

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p1_4,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3,__zpabi_f1__p2_4,__zpabi_f1__p3 locals=__local_f1__0 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,X
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__l0_1,__local_f2__l0_0,__local_f2__2,__local_f2__3,__local_f2__it_1,__local_f2__it_0,__local_f2__4,__local_f2__5 indirect=false in_cycle=false carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p1_2,__zpabi_f3__p1_3 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3 indirect=false in_cycle=false param_regs=A,X,,,,
; def f4 params= locals= indirect=true in_cycle=false carry=true
; def f5 params= locals=__local_f5__0,__local_f5__1 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f4
; call f2 -> f3
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=1 at=auto def=true init=$41 weight=20
; zp g0 size=4 at=auto def=true init=$49,$00,$00,$00 weight=18
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f5:
   SUBROUTINE

   ; prologue: 9 arg bytes, 0 local bytes
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f5__1
   STA   __local_f5__0
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .or_true@0
   LDY   #$04
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   ORA   __local_f5__1
   ORA   __local_f5__0
   BEQ   .or_end@1
.or_true@0:
.or_end@1:
   LDY   #$0A
   LDA   (FP),Y
   SEC
   LDY   #$04
   SBC   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0B
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

f4:
   SUBROUTINE

   ; prologue: 12 arg bytes, 11 local bytes, 11 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0C
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

   LDA   #$64
   SEC
   LDY   #$10
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   HARGS+2
   LDA   #$64
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $84
   LDA   HARGS+5
   STA   $83
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   $82
   STA   $81
   LDA   $84
   STA   $CA
   LDA   $83
   STA   $C9
   LDA   $82
   STA   $C8
   STA   $C7
   AND   #$01
   STA   $C6
   LDY   #$17
   LDA   (FP),Y
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   $C5
   STA   $C4
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .or_true@2
   LDA   #$00
   STA   $C3
   STA   $C2
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   $C3
   LDA   #$00
   STA   $C2
.or_end@3:
   LDA   #<f5
   STA   $C0
   LDA   #>f5
   STA   $C1
   LDA   g2
   STA   $80
   LDY   #$0E
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   $80
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $83
   LDA   HARGS+5
   STA   $82
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   $81
   LDA   $83
   STA   $88
   LDA   $82
   STA   $87
   LDA   $81
   STA   $86
   AND   #$1F
   STA   $85
   LDY   #$0F
   LDA   (FP),Y
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   $84
   STA   $83
   LDY   #$0E
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   STA   $80
   LDA   $84
   AND   #$01
   STA   $82
   LDA   $81
   SEC
   SBC   $CA
   STA   $81
   LDA   $80
   SBC   $C9
   STA   $80
   LDA   $84
   SBC   $C8
   STA   $84
   LDA   $83
   SBC   $C7
   STA   $83
   LDA   $82
   SBC   $C6
   STA   $82
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$64
   LDY   #$01
   STA   (SSP),Y
   LDA   $C3
   INY
   STA   (SSP),Y
   LDA   $C2
   INY
   STA   (SSP),Y
   LDA   $88
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
   AND   #$FF
   STA   $80
   LDA   #$07
   EOR   $80
   CLC
   ADC   g2
   STA   g2
   LDA   $C4
   ORA   $C5
   LDY   #$17
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@10
   LDA   #$01
.cmp_end@10:
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
   INY
   LDA   (FP),Y
   STA   $CA
   CLC
   LDA   FP
   ADC   #$19
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

f3:
   SUBROUTINE

   LDA   g0
   SEC
   SBC   __zpabi_f3__p1_0
   LDA   g0+1
   SBC   __zpabi_f3__p1_1
   LDA   g0+2
   SBC   __zpabi_f3__p1_2
   LDA   g0+3
   SBC   __zpabi_f3__p1_3
   LDA   #$00
   ROL
   STA   __local_f3__2
   LDA   g2
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f3__0
   LDA   g2
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f3__0
   STA   HARGS+3
   LDA   __local_f3__2
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   HARGS+6
   ORA   HARGS+7
   ORA   __local_f3__1
   ORA   __local_f3__0
   BNE   .or_true@4
   LDA   g2
   BEQ   .or_end@5
.or_true@4:
.or_end@5:
   LDA   #$07
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f2:
   SUBROUTINE

   LDA   #$00
   CMP   g0+3
   BNE   .vthread@0
   CMP   g0+2
   BNE   .vthread@0
   CMP   g0+1
   BNE   .vthread@0
   LDA   #$01
   CMP   g0
   BEQ   .cmp_true@15
   LDA   #$00
.vthread@0:
.cmp_true@15:
   STA   __local_f2__l0_0
   LDA   #$00
   STA   __local_f2__l0_1
   STA   __local_f2__it_0
   LDY   #$00
   STY   __local_f2__it_1
.loop@0_start:
   LDA   __local_f2__it_0
   SEC
   SBC   #$03
   LDA   __local_f2__it_1
   SBC   #$00
   BVC   .jcmp_novf@18
   EOR   #$80
.jcmp_novf@18:
   BPL   .loop@0_break
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .lnot_true@19
   LDA   #$00
   BEQ   .lnot_end@20
.lnot_true@19:
   LDA   #$01
.lnot_end@20:
   STA   __zpabi_f3__p1_0
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __zpabi_f3__p1_2
   STA   __zpabi_f3__p1_3
   LDA   #$00
   STA   __zpabi_f3__p1_1
   LDX   #$00
   LDA   g2
   JSR   f3
   LDA   __local_f2__it_0
   STA   __local_f2__1
   LDA   __local_f2__it_1
   STA   __zpabi_f3__p1_0
   LDA   HARGS+8
   CLC
   ADC   __local_f2__1
   STA   __local_f2__1
   LDA   HARGS+9
   ADC   __zpabi_f3__p1_0
   STA   __zpabi_f3__p1_0
   LDA   __local_f2__l0_0
   CLC
   ADC   __local_f2__1
   STA   __local_f2__1
   LDA   __local_f2__l0_1
   ADC   __zpabi_f3__p1_0
   STA   __local_f2__l0_1
   LDA   __local_f2__1
   STA   __local_f2__l0_0
   INC   __local_f2__it_0
   BNE   .loop@0_start
   INC   __local_f2__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g2
   AND   #$02
   BEQ   .cmp_end@23
   SEC
   RTS
.cmp_end@23:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   LDA   g2
   AND   #$01
   RTS

f0:
   SUBROUTINE

   ; prologue: 6 arg bytes, 7 local bytes, 7 callee-saved bytes
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
   BEQ   .lnot_true@24
   LDA   #$00
   BEQ   .lnot_end@25
.lnot_true@24:
   LDA   #$01
.lnot_end@25:
   STA   $C6
   LDY   #$0E
   LDA   (FP),Y
   STA   HARGS
   LDY   #$0C
   LDA   (FP),Y
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   $C5
   LDA   #$FF
   CLC
   LDY   #$0C
   ADC   (FP),Y
   STA   $84
   LDA   #$7F
   ADC   #$00
   STA   $83
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   $82
   STA   $81
   STA   $80
   LDA   $84
   STA   $C4
   LDA   $83
   STA   $C3
   LDA   $82
   STA   $C2
   STA   $C1
   AND   #$01
   STA   $C0
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   $83
   LDA   HARGS+6
   STA   $82
   LDA   HARGS+7
   STA   $81
   LDA   HARGS+8
   STA   $80
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C6
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $C5
   INY
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   LDA   $C3
   INY
   STA   (SSP),Y
   LDA   $C2
   INY
   STA   (SSP),Y
   LDA   $C1
   INY
   STA   (SSP),Y
   LDA   $C0
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
   JSR   f4
   BCC   .if_else@9
   LDY   #$0C
   LDA   (FP),Y
   CLC
   ADC   #$03
   JMP   .if_end@8
.if_else@9:
   LDY   #$0C
   LDA   (FP),Y
   EOR   #$05
.if_end@8:
   CLC
   LDY   #$0C
   ADC   (FP),Y
   STA   $81
   LDA   #$00
   INY
   ADC   (FP),Y
   STA   $83
   LDA   $81
   CLC
   ADC   g2
   STA   $82
   LDA   $83
   ADC   #$00
   STA   $83
   LDA   #$00
   ADC   #$00
   STA   $81
   LDA   #$00
   ADC   #$00
   STA   $80
   LDA   $82
   STA   HARGS+8
   LDA   $83
   STA   HARGS+9
   LDA   $81
   STA   HARGS+10
   LDA   $80
   AND   #$1F
   STA   HARGS+11

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
   CLC
   LDA   FP
   ADC   #$0F
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

   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$D8
   DEY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$54
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   HARGS+8
   STA   $88
   LDA   HARGS+9
   STA   $87
   LDA   HARGS+10
   STA   $86
   LDA   HARGS+11
   STA   $85
   LDA   g1
   STA   $81
   LDA   g1+1
   STA   $80
   LDA   g0
   EOR   $81
   STA   $83
   LDA   g0+1
   EOR   $80
   STA   $82
   LDA   g0+2
   STA   $81
   LDA   g0+3
   STA   $84
   LDA   $83
   EOR   g2
   STA   $83
   LDA   $84
   STA   $80
   LDA   $88
   EOR   $83
   STA   $C1
   LDA   $87
   EOR   $82
   STA   $C0
   LDA   $86
   EOR   $81
   STA   HARGS+2
   LDA   $85
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
   DC.W  $0012

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   LDA   #$49
   STA   g0
   LDA   #$41
   STA   g2
   RTS
