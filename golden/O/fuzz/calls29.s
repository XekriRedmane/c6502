SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
__local_f2__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__local_f2__1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__local_f2__2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__local_f2__3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f1__p1	EQU	$84
__zpabi_f1__pp_0	EQU	$85
__zpabi_f1__pp_1	EQU	$86
__local_f1__0	EQU	$87
__local_f1__1	EQU	$88
__local_f1__2	EQU	$89
__local_f1__p2_1	EQU	$8D
__local_f1__p2_0	EQU	$8E

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__p2_1,__local_f1__p2_0 indirect=false in_cycle=false param_regs=,,,,,A,X,,
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$16,$00 weight=6
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

f2:
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

   LDA   g0
   STA   __local_f2__3
   LDA   g0+1
   STA   __local_f2__2
   LDY   #$07
   LDA   (FP),Y
   STA   __local_f2__0
   LDA   #$7F
   INY
   AND   (FP),Y
   STA   __local_f2__1
   LDA   __local_f2__0
   SEC
   SBC   __local_f2__3
   STA   __local_f2__0
   LDA   __local_f2__1
   SBC   __local_f2__2
   TAX
   LDA   __local_f2__0

   ; epilogue
   PHA
   TXA
   PHA
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
   PLA
   TAX
   PLA
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p2_0
   STX   __local_f1__p2_1
   LDA   g0
   SEC
   SBC   g0
   STA   __local_f1__1
   LDA   g0+1
   SBC   g0+1
   STA   __local_f1__0
   LDY   #$00
   LDA   (__zpabi_f1__pp_0),Y
   CLC
   ADC   __local_f1__1
   STA   __local_f1__1
   INY
   LDA   (__zpabi_f1__pp_0),Y
   ADC   __local_f1__0
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   DEY
   STA   (__zpabi_f1__pp_0),Y
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__2
   SEC
   SBC   __local_f1__p2_0
   INY
   LDA   (__zpabi_f1__pp_0),Y
   SBC   __local_f1__p2_1
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f1__1
   LDA   __local_f1__2
   EOR   #$E8
   STA   __local_f1__0
   LDA   __local_f1__1
   EOR   __local_f1__0
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

   LDA   g1
   AND   #$07
   TAX
   LDA   #$E8
   STA   $80
   LDA   #$03
   STA   $81
   CPX   #$00
   BEQ   .vthread@0
.shift_loop@1:
   LDA   $81
   ASL
   ROR   $81
   ROR   $80
   DEX
   BNE   .shift_loop@1
   LDA   #$03
.vthread@0:
   CLC
   SBC   $80
   LDA   #$00
   SBC   $81
   LDA   #$00
   ROL
   STA   $C0
   LDA   #<f2
   STA   $84
   LDA   #>f2
   STA   $85
   LDA   g1
   STA   $83
   LDA   #$00
   STA   $82
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C0
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
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
   LDA   $84
   STA   DPTR
   LDA   $85
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $81
   CLC
   ADC   g0
   STA   g0
   BCC   .carry_inc@0
   INC   g0+1
   BNE   .carry_inc@0
   INC   g0+2
   BNE   .carry_inc@0
   INC   g0+3
.carry_inc@0:
   LDA   #$FF
   CLC
   SBC   #$FF
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   $80
   LDA   $81
   SEC
   SBC   $80
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
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
   STA   $85
   STX   $84
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   $81
   STA   $80
   LDA   g0
   EOR   $83
   STA   $83
   LDA   g0+1
   EOR   $82
   STA   $82
   LDA   g0+2
   EOR   $81
   STA   $81
   LDA   g0+3
   EOR   $80
   STA   $80
   LDA   $85
   EOR   $83
   STA   $C1
   TXA
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
   DC.L  $0000009B

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   LDA   #$16
   STA   g1
   RTS
