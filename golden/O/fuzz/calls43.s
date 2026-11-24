SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_f2__0	EQU	$80
__zpabi_f0__p1_0	EQU	$80
__local_f2__1	EQU	$81
__zpabi_f0__p1_1	EQU	$81
__local_f2__2	EQU	$82
__zpabi_f0__p1_2	EQU	$82
__local_f2__3	EQU	$83
__zpabi_f0__p2_0	EQU	$83
__zpabi_f0__p2_1	EQU	$84
__zpabi_f0__p2_2	EQU	$85
__zpabi_f0__p2_3	EQU	$86
__zpabi_f0__p3_0	EQU	$87
__zpabi_f0__p3_1	EQU	$88
__zpabi_f0__p3_2	EQU	$89
__local_main__0	EQU	$8A
__local_main__1	EQU	$8B
__local_main__2	EQU	$8C
__local_main__3	EQU	$8D
__local_main__4	EQU	$8E
__local_main__5	EQU	$8F
__local_main__8	EQU	$92

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3,__zpabi_f0__p3_0,__zpabi_f0__p3_1,__zpabi_f0__p3_2 locals=__local_f0__0 indirect=false in_cycle=false param_regs=A,,,,,,,,,, carry=true
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item g1 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

f2:
   SUBROUTINE

   ; prologue: 3 arg bytes, 0 local bytes
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f2__3
   LDY   #$04
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_f2__2
   LDA   g1
   STA   __local_f2__1
   LDA   g1+1
   STA   __local_f2__0
   LDA   __local_f2__3
   CLC
   ADC   __local_f2__1
   STA   __local_f2__1
   LDA   __local_f2__2
   ADC   __local_f2__0
   STA   __local_f2__0
   LDY   #$04
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f2__1
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f2__0
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   STA   HARGS+8
   DEY
   STY   HARGS+9
   STA   HARGS+10

   ; epilogue
   CLC
   LDA   FP
   ADC   #$05
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

   ; prologue: 0 arg bytes, 3 local bytes, 1 callee-saved bytes
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

   LDA   g0
   STA   $81
   LDA   g1+3
   CMP   g1+3
   BNE   .thread@0
   LDA   g1+2
   CMP   g1+2
   BNE   .thread@0
   LDA   g1+1
   CMP   g1+1
   BNE   .thread@0
   LDA   g1
   CMP   g1
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   STA   $80
   LDA   $81
   SEC
   SBC   $80
   AND   #$07
   EOR   #$04
   SEC
   SBC   #$04
   STA   $C0
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$03
   STA   (FP),Y
   LDA   #<f2
   STA   $83
   LDA   #>f2
   STA   $84
   LDA   g0
   STA   $82
   CLC
   LDA   FP
   ADC   #$02
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $82
   LDY   #$01
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $83
   STA   DPTR
   LDA   $84
   STA   DPTR+1
   JSR   icall
   LDA   $C0
   EOR   HARGS+8
   AND   #$07
   EOR   #$04
   SEC
   SBC   #$04
   STA   $81
   LDY   #$02
   LDA   (FP),Y
   CLC
   ADC   $81
   AND   #$07
   EOR   #$04
   SEC
   SBC   #$04
   STA   $83
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $82
   STA   $81
   STA   $80
   LDA   g1
   SEC
   SBC   $83
   LDA   g1+1
   SBC   $82
   LDA   g1+2
   SBC   $81
   LDA   g1+3
   SBC   $80
   LDA   #$00
   ROL
   ORA   $83

   ; epilogue
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$04
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

   LDA   __zpabi_f0__p1_2
   ORA   __zpabi_f0__p1_1
   ORA   __zpabi_f0__p1_0
   BEQ   .cmp_end@9
   SEC
   RTS
.cmp_end@9:
   CMP   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$D4
   STA   __zpabi_f0__p1_0
   LDA   #$66
   STA   __zpabi_f0__p2_0
   LDA   #$C0
   STA   __zpabi_f0__p3_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   STA   __zpabi_f0__p3_1
   STA   __zpabi_f0__p3_2
   LDA   #$21
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g0+1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g1+2
   STA   __local_main__1
   LDA   __local_main__4
   EOR   g1+3
   STA   __local_main__0
   LDA   __local_main__8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   #$00
   EOR   __local_main__2
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

g0:
   DC.W  $0076

g1:
   DC.L  $000000AB
