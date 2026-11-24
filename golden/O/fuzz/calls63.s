SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p0_0	EQU	$80
__zpabi_f4__p2	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f4__pp_0	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f4__pp_1	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f4__p0_0	EQU	$83
__zpabi_f0__pp_0	EQU	$84
__zpabi_f4__p0_1	EQU	$84
__zpabi_f0__pp_1	EQU	$85
__zpabi_f5__p0_0	EQU	$85
__local_main__0	EQU	$86
__zpabi_f5__p0_1	EQU	$86
__local_main__1	EQU	$87
__zpabi_f5__p0_2	EQU	$87
__local_main__2	EQU	$88
__zpabi_f5__p0_3	EQU	$88
__local_main__3	EQU	$89
__zpabi_f5__p1	EQU	$89
__local_main__4	EQU	$8A
__zpabi_f5__p3_0	EQU	$8A
__local_main__mbox	EQU	$8B
__zpabi_f5__p3_1	EQU	$8B
__local_f2__0	EQU	$8C
__local_f2__1	EQU	$8D
__local_f2__2	EQU	$8E
__local_f0__0	EQU	$8F
__local_f2__it_1	EQU	$8F
__local_f0__1	EQU	$90
__local_f2__it_0	EQU	$90
__local_f0__2	EQU	$91
__local_f2__l2	EQU	$91
__local_f0__3	EQU	$92
__local_f2__3	EQU	$92
__local_f0__4	EQU	$93
__local_f0__5	EQU	$94
__local_f0__6	EQU	$95
__local_f2__6	EQU	$95
__local_f0__7	EQU	$96
__local_f2__7	EQU	$96
__local_f0__8	EQU	$97
__local_f2__8	EQU	$97
__local_f2__9	EQU	$98
__local_f2__10	EQU	$99
__local_f2__11	EQU	$9A
__local_f2__12	EQU	$9B
__local_f2__13	EQU	$9C
__local_f2__14	EQU	$9D
__local_f2__15	EQU	$9E
__local_f2__box4	EQU	$9F
__local_f4__1	EQU	$A2
__local_f4__2	EQU	$A3
__local_f4__3	EQU	$A4
__local_f4__5	EQU	$A6
__local_f4__6	EQU	$A7
__local_f4__7	EQU	$A8
__local_f4__8	EQU	$A9
__local_f4__9	EQU	$AA
__local_f4__10	EQU	$AB
__local_f4__11	EQU	$AC
__local_f4__12	EQU	$AD
__local_f4__13	EQU	$AE
__local_f4__p1	EQU	$AF
__local_f5__0	EQU	$B0
__local_f5__1	EQU	$B1
__local_f5__2	EQU	$B2
__local_f5__3	EQU	$B3
__local_f5__4	EQU	$B4

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__8 indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__it_1,__local_f2__it_0,__local_f2__l2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__8,__local_f2__9,__local_f2__10,__local_f2__11,__local_f2__12,__local_f2__13,__local_f2__14,__local_f2__15,__local_f2__box4,__local_f2__17 indirect=false in_cycle=false
; def f3 params= locals= indirect=false in_cycle=false
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p1,__zpabi_f4__p2,__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6,__local_f4__7,__local_f4__8,__local_f4__9,__local_f4__10,__local_f4__11,__local_f4__12,__local_f4__13,__local_f4__p1 indirect=false in_cycle=false param_regs=,,A,,,
; def f5 params=__zpabi_f5__p0_0,__zpabi_f5__p0_1,__zpabi_f5__p0_2,__zpabi_f5__p0_3,__zpabi_f5__p1,__zpabi_f5__p2,__zpabi_f5__p3_0,__zpabi_f5__p3_1 locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3,__local_f5__4,__local_f5__5,__local_f5__6 indirect=false in_cycle=false param_regs=,,,,,A,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__mbox,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f1 -> f5
; call f1 -> rec
; call f2 -> f4
; call f2 -> f5
; call f3 -> rec
; call f4 -> f5
; call main -> f0
; call rec -> rec
; share member=f4:__zpabi_f4__p0_0,__zpabi_f4__p0_1 holder=f2 live=f2:f4 forward=f2>f4 writer=f2
; item rec section=.text local=false zero=false
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@run3 section=.text local=true zero=false
; item f2@run2 section=.text local=true zero=false
; item f2@run1 section=.text local=true zero=false
; item f2@run0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=4 at=auto def=true init=$40,$00,$00,$00 weight=26
; runtime hargs_size=32 hargs_need=23
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

f5:
   SUBROUTINE

   LDA   __zpabi_f5__p3_1
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f5__4
   STA   __local_f5__3
   STA   __local_f5__2
   STA   __local_f5__1
   AND   #$7F
   STA   __local_f5__0
   LDA   __zpabi_f5__p3_0
   CLC
   SBC   __zpabi_f5__p3_0
   LDA   __zpabi_f5__p3_1
   SBC   #$00
   LDA   __local_f5__4
   SBC   #$00
   LDA   __local_f5__3
   SBC   #$00
   LDA   __local_f5__2
   SBC   #$00
   LDA   __local_f5__1
   SBC   #$00
   LDA   __local_f5__0
   SBC   #$00
   RTS

f4:
   SUBROUTINE

   STA   __local_f4__p1
   LDA   g0+3
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f4__2
   STA   __local_f4__1
   AND   #$7F
   STA   __zpabi_f5__p0_0
   LDA   g1
   SEC
   SBC   g0
   STA   __local_f4__6
   LDA   g1+1
   SBC   g0+1
   STA   __local_f4__5
   LDA   g1+2
   SBC   g0+2
   STA   __zpabi_f5__p3_0
   LDA   g1+3
   SBC   g0+3
   STA   __local_f4__3
   LDA   g1+4
   SBC   __local_f4__2
   STA   __local_f4__2
   LDA   g1+5
   SBC   __local_f4__1
   STA   __local_f4__1
   LDA   g1+6
   SBC   __zpabi_f5__p0_0
   STA   __zpabi_f5__p0_0
   LDA   __local_f4__6
   STA   __local_f4__13
   LDA   __local_f4__5
   STA   __local_f4__12
   LDA   __zpabi_f5__p3_0
   STA   __local_f4__11
   LDA   __local_f4__3
   STA   __local_f4__10
   LDA   __local_f4__2
   STA   __local_f4__9
   LDA   __local_f4__1
   STA   __local_f4__8
   LDA   __zpabi_f5__p0_0
   AND   #$7F
   STA   __local_f4__7
   LDA   g0+3
   BNE   .thread@0
   LDA   g0+2
   BNE   .thread@0
   LDA   g0+1
   CMP   #$7F
   BNE   .thread@0
   LDA   g0
   CMP   #$FF
   BEQ   .cmp_true@5
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   __local_f4__5
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_f5__p3_0
   STA   __local_f4__3
   STA   __local_f4__2
   STA   __local_f4__1
   LDA   __local_f4__5
   STA   __local_f4__6
   LDA   __zpabi_f5__p3_0
   AND   #$7F
   STA   __zpabi_f5__p0_0
   LDA   __local_f4__6
   SEC
   SBC   __local_f4__13
   LDA   #$00
   SBC   __local_f4__12
   LDA   __zpabi_f5__p3_0
   SBC   __local_f4__11
   LDA   __local_f4__3
   SBC   __local_f4__10
   LDA   __local_f4__2
   SBC   __local_f4__9
   LDA   __local_f4__1
   SBC   __local_f4__8
   LDA   __zpabi_f5__p0_0
   SBC   __local_f4__7
   LDA   #$00
   ROL
   STA   __zpabi_f5__p3_0
   LDY   #$00
   LDA   (__zpabi_f4__pp_0),Y
   CLC
   ADC   g0
   STA   __local_f4__1
   INY
   LDA   (__zpabi_f4__pp_0),Y
   ADC   g0+1
   STA   (__zpabi_f4__pp_0),Y
   LDA   __local_f4__1
   DEY
   STA   (__zpabi_f4__pp_0),Y
   LDA   __local_f4__p1
   SEC
   SBC   __zpabi_f4__p0_0
   LDA   #$00
   SBC   __zpabi_f4__p0_1
   LDA   #$00
   ROL
   STA   __zpabi_f5__p0_0
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_f5__p0_2
   STA   __zpabi_f5__p0_3
   LDA   #$01
   STA   __zpabi_f5__p1
   LDA   #$00
   STA   __zpabi_f5__p0_1
   STA   __zpabi_f5__p3_1
   JSR   f5
   LDY   #$00
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__1
   INY
   LDA   (__zpabi_f4__pp_0),Y
   STA   __zpabi_f5__p0_0
   CMP   __zpabi_f5__p0_0
   BNE   .and_false@3
   LDA   __local_f4__1
   CMP   __local_f4__1
   BNE   .and_false@3
   LDA   __zpabi_f4__p0_0
   ORA   __zpabi_f4__p0_1
   BEQ   .and_false@3
   LDA   #$01
   STA   __local_f4__1
   LDA   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_f4__1
.and_end@4:
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f4__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f3:
   SUBROUTINE

   ; prologue: 7 arg bytes, 11 local bytes, 11 callee-saved bytes
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

   LDY   #$14
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDA   #$00
   BEQ   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   STA   $85
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   $84
   LDA   $85
   STA   $CA
   LDA   #$00
   STA   $C9
   LDA   $84
   STA   $C8
   STA   $C7
   LDA   g0
   STA   $86
   LDA   g0+1
   STA   $85
   LDA   g0+2
   STA   $84
   LDA   g0+3
   STA   $83
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   $82
   STA   $81
   STA   $80
   LDA   $86
   STA   HARGS
   LDA   $85
   STA   HARGS+1
   LDA   $84
   STA   HARGS+2
   LDA   $83
   STA   HARGS+3
   LDA   $82
   STA   HARGS+4
   STA   HARGS+5
   AND   #$7F
   STA   HARGS+6
   LDA   g1
   STA   HARGS+8
   LDA   g1+1
   STA   HARGS+9
   LDA   g1+2
   STA   HARGS+10
   LDA   g1+3
   STA   HARGS+11
   LDA   g1+4
   STA   HARGS+12
   LDA   g1+5
   STA   HARGS+13
   LDA   g1+6
   STA   HARGS+14
   LDA   #$00
   STA   HARGS+7
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   STA   $86
   LDA   HARGS+17
   STA   $85
   LDA   HARGS+18
   STA   $84
   LDA   HARGS+19
   STA   $83
   LDA   HARGS+20
   STA   $82
   LDA   HARGS+21
   STA   $81
   LDA   HARGS+22
   STA   $80
   LDA   $86
   STA   $8D
   LDA   $85
   STA   $8C
   LDA   $84
   STA   $8B
   LDA   $83
   STA   $8A
   LDA   $82
   STA   $89
   LDA   $81
   STA   $88
   LDA   $80
   AND   #$7F
   STA   $87
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .lnot_true@21
   LDA   #$00
   BEQ   .lnot_end@22
.lnot_true@21:
   LDA   #$01
.lnot_end@22:
   STA   $85
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   $84
   STA   $83
   STA   $82
   STA   $81
   LDA   $85
   STA   $86
   LDA   $84
   AND   #$7F
   STA   $80
   LDA   $8D
   SEC
   SBC   $86
   LDA   $8C
   SBC   #$00
   LDA   $8B
   SBC   $84
   LDA   $8A
   SBC   $83
   LDA   $89
   SBC   $82
   LDA   $88
   SBC   $81
   LDA   $87
   SBC   $80
   LDA   #$00
   ROL
   STA   $85
   LDA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   $84
   STA   $83
   STA   $82
   STA   $81
   LDA   $85
   STA   $C6
   LDA   #$00
   STA   $C5
   LDA   $84
   STA   $C4
   STA   $C3
   STA   $C2
   STA   $C1
   AND   #$7F
   STA   $C0
   LDA   $CA
   ORA   $C9
   ORA   $C8
   ORA   $C7
   BEQ   .lnot_true@27
   LDA   #$00
   BEQ   .lnot_end@28
.lnot_true@27:
   LDA   #$01
.lnot_end@28:
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   g1
   ORA   #$01
   STA   HARGS+8
   LDA   g1+1
   STA   HARGS+9
   LDA   g1+2
   STA   HARGS+10
   LDA   g1+3
   STA   HARGS+11
   LDA   g1+4
   STA   HARGS+12
   LDA   g1+5
   STA   HARGS+13
   LDA   g1+6
   STA   HARGS+14
   LDA   $C6
   STA   HARGS
   LDA   $C5
   STA   HARGS+1
   LDA   $C4
   STA   HARGS+2
   LDA   $C3
   STA   HARGS+3
   LDA   $C2
   STA   HARGS+4
   LDA   $C1
   STA   HARGS+5
   LDA   $C0
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   STA   $86
   LDA   HARGS+17
   STA   $85
   LDA   HARGS+18
   STA   $84
   LDA   HARGS+19
   STA   $83
   LDA   HARGS+20
   STA   $82
   LDA   HARGS+21
   STA   $81
   LDA   HARGS+22
   STA   $80
   LDA   $86
   ORA   $85
   ORA   $84
   ORA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BEQ   .lnot_true@29
   LDA   #$00
   BEQ   .lnot_end@30
.lnot_true@29:
   LDA   #$01
.lnot_end@30:

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
   CLC
   LDA   FP
   ADC   #$14
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

   JSR   f2@run1
   BEQ   .and_false@5
   JSR   f2@run0
   BEQ   .and_false@5
   LDA   #$01
   STA   __local_f2__3
   LDA   #$00
   BEQ   .and_end@6
.and_false@5:
   LDA   #$00
   STA   __local_f2__3
.and_end@6:
   ORA   #$00
   BPL   .sx_done@32
   LDA   #$FF
.sx_done@32:
   STA   __local_f2__it_0
   STA   __local_f2__it_1
   STA   __local_f2__2
   STA   __local_f2__1
   AND   #$7F
   STA   __local_f2__0
   LDA   __local_f2__3
   SEC
   JSR   f2@run3
   LDA   __local_f2__it_0
   SBC   g1+2
   LDA   __local_f2__it_1
   SBC   g1+3
   LDA   __local_f2__2
   SBC   g1+4
   LDA   __local_f2__1
   SBC   g1+5
   LDA   __local_f2__0
   JSR   f2@run2
   STA   __local_f2__15
   LDA   #$00
   BPL   .sx_done@34
   LDA   #$FF
.sx_done@34:
   STA   __local_f2__14
   STA   __local_f2__13
   JSR   f2@run0
   BNE   .or_true@7
   JSR   f2@run1
   BNE   .or_true@7
   LDA   #$00
   STA   __local_f2__3
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   STA   __local_f2__3
   LDA   #$00
.or_end@8:
   ORA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   LDX   __local_f2__3
   STX   __local_f2__12
   LDX   #$00
   STX   __local_f2__11
   STA   __local_f2__10
   STA   __local_f2__9
   STA   __local_f2__8
   STA   __local_f2__7
   AND   #$07
   STA   __local_f2__6
   LDA   g0
   STA   __local_f2__l2
   LDA   __local_f2__15
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f2__15
   STA   HARGS
   LDA   __local_f2__14
   STA   HARGS+2
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   g0+3
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   __local_f2__2
   STA   __local_f2__1
   AND   #$7F
   STA   __local_f2__0
   LDA   g0
   SEC
   SBC   g1
   LDA   g0+1
   SBC   g1+1
   LDA   g0+2
   SBC   g1+2
   LDA   g0+3
   SBC   g1+3
   LDA   __local_f2__2
   SBC   g1+4
   LDA   __local_f2__1
   SBC   g1+5
   LDA   __local_f2__0
   JSR   f2@run2
   STA   __local_f2__0
   LDA   #$00
   SEC
   SBC   __local_f2__15
   LDA   #$00
   ROL
   STA   __zpabi_f5__p3_0
   LDA   __local_f2__15
   STA   __zpabi_f5__p0_0
   LDA   #$00
   STA   __zpabi_f5__p0_1
   LDA   __local_f2__14
   STA   __zpabi_f5__p0_2
   LDA   __local_f2__13
   STA   __zpabi_f5__p0_3
   LDA   HARGS+8
   STA   __zpabi_f5__p1
   LDA   #$00
   STA   __zpabi_f5__p3_1
   LDA   __local_f2__0
   JSR   f5
   LDA   #$00
   ROL
   STA   __local_f2__0
   LDA   __local_f2__15
   EOR   __local_f2__0
   STA   __local_f2__box4
   LDA   #$00
   STA   __local_f2__box4+1
   LDA   __local_f2__12
   STA   __zpabi_f4__p0_0
   LDA   __local_f2__11
   STA   __zpabi_f4__p0_1
   LDA   #$00
   STA   __local_f2__it_0
   LDY   #$00
   STY   __local_f2__it_1
.loop@0_start:
   LDA   __local_f2__it_0
   SEC
   SBC   #$03
   LDA   __local_f2__it_1
   SBC   #$00
   BVC   .jcmp_novf@39
   EOR   #$80
.jcmp_novf@39:
   BPL   .loop@0_break
   LDA   __local_f2__12
   ORA   __local_f2__11
   ORA   __local_f2__10
   ORA   __local_f2__9
   ORA   __local_f2__8
   ORA   __local_f2__7
   ORA   __local_f2__6
   BEQ   .and_false@9
   JSR   f2@run1
   BEQ   .and_false@9
   LDA   #$01
   STA   __zpabi_f4__p2
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   STA   __zpabi_f4__p2
.and_end@10:
   LDA   #<__local_f2__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f2__box4
   STA   __zpabi_f4__pp_1
   LDA   #$FD
   JSR   f4
   LDA   HARGS+8
   CLC
   ADC   __local_f2__it_0
   CLC
   ADC   __local_f2__l2
   STA   __local_f2__l2
   INC   __local_f2__it_0
   BNE   .loop@0_start
   INC   __local_f2__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f2__12
   RTS

f2@run3:
   SUBROUTINE

   SBC   g1
   LDA   #$00
   SBC   g1+1
   RTS

f2@run2:
   SUBROUTINE

   SBC   g1+6
   LDA   #$00
   ROL
   RTS

f2@run1:
   SUBROUTINE

   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   RTS

f2@run0:
   SUBROUTINE

   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   ORA   g1+4
   ORA   g1+5
   ORA   g1+6
   RTS

f1:
   SUBROUTINE

   ; prologue: 11 arg bytes, 0 local bytes
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

   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $81
   LDY   #$06
   LDA   (FP),Y
   SEC
   SBC   #$07
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   CLC
   ADC   $82
   STA   $80
   BCC   .carry_inc@0
   INC   $81
.carry_inc@0:
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$01
   STA   (DPTR),Y
   LDA   g0
   AND   #$07
   TAX
   LDA   #$03
   STA   $8C
   LDA   #$00
   STA   $8D
   CPX   #$00
   BEQ   .shift_done@42
.shift_loop@41:
   ASL   $8C
   ROL   $8D
   DEX
   BNE   .shift_loop@41
.shift_done@42:
   LDA   $8D
   BMI   .sx_neg@43
   LDA   #$00
   BEQ   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   STA   $83
   STA   $82
   LDY   #$07
   LDA   (FP),Y
   BMI   .sx_neg@45
   LDA   #$00
   BEQ   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   $81
   STA   $80
   LDA   g0+3
   CMP   $80
   BNE   .thread@1
   LDA   g0+2
   CMP   $81
   BNE   .thread@1
   LDA   g0+1
   LDY   #$07
   CMP   (FP),Y
   BNE   .thread@1
   LDA   g0
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@47
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   STA   $81
   LDY   #$08
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@11
   LDA   #$00
   STA   $80
   JMP   .or_end@12
.or_true@11:
   LDA   #$01
   STA   $80
.or_end@12:
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __zpabi_f5__p3_0
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __zpabi_f5__p3_1
   LDA   $8C
   STA   __zpabi_f5__p0_0
   LDA   $8D
   STA   __zpabi_f5__p0_1
   LDA   $83
   STA   __zpabi_f5__p0_2
   LDA   $82
   STA   __zpabi_f5__p0_3
   LDA   $81
   STA   __zpabi_f5__p1
   LDA   $80
   JSR   f5
   BCC   .if_else@14
   LDY   #$04
   LDA   (FP),Y
   CLC
   ADC   #$03
   STA   $8D
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $8C
   LDA   #$00
   ADC   #$00
   STA   $84
   LDA   #$00
   ADC   #$00
   STA   $83
   LDA   #$00
   ADC   #$00
   STA   $82
   LDA   #$00
   ADC   #$00
   STA   $81
   LDA   #$00
   ADC   #$00
   AND   #$07
   STA   $80
   JMP   .if_end@13
.if_else@14:
   LDY   #$04
   LDA   (FP),Y
   EOR   #$05
   STA   $8D
   INY
   LDA   (FP),Y
   STA   $8C
   LDA   #$00
   STA   $84
   LDY   #$00
   STY   $83
   STY   $82
   STY   $81
   STA   $80
.if_end@13:
   LDA   g1
   ORA   #$01
   STA   HARGS+8
   LDA   g1+1
   STA   HARGS+9
   LDA   g1+2
   STA   HARGS+10
   LDA   g1+3
   STA   HARGS+11
   LDA   g1+4
   STA   HARGS+12
   LDA   g1+5
   STA   HARGS+13
   LDA   g1+6
   STA   HARGS+14
   LDA   $8D
   STA   HARGS
   LDA   $8C
   STA   HARGS+1
   LDA   $84
   STA   HARGS+2
   LDA   $83
   STA   HARGS+3
   LDA   $82
   STA   HARGS+4
   LDA   $81
   STA   HARGS+5
   LDA   $80
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   STA   $81
   LDA   HARGS+17
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
   BMI   .sx_neg@50
   LDA   #$00
   BEQ   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   STA   $81
   STA   $80
   LDY   #$08
   LDA   (FP),Y
   LDY   #$06
   ORA   (FP),Y
   STA   $83
   LDY   #$09
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   STA   $82
   LDY   #$0A
   LDA   (FP),Y
   ORA   $81
   STA   $81
   INY
   LDA   (FP),Y
   ORA   $80
   ORA   $81
   ORA   $82
   ORA   $83
   BEQ   .cmp_end@52
   LDA   #$01
.cmp_end@52:

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0D
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

f0:
   SUBROUTINE

   LDA   __zpabi_f0__p0_3
   BMI   .sx_neg@53
   LDA   #$00
   BEQ   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   STA   __local_f0__2
   STA   __local_f0__1
   AND   #$7F
   STA   __local_f0__0
   LDA   __zpabi_f0__p0_0
   SEC
   SBC   g1
   STA   __local_f0__6
   LDA   __zpabi_f0__p0_1
   SBC   g1+1
   STA   __local_f0__5
   LDA   __zpabi_f0__p0_2
   SBC   g1+2
   STA   __local_f0__4
   LDA   __zpabi_f0__p0_3
   SBC   g1+3
   STA   __local_f0__3
   LDA   __local_f0__2
   SBC   g1+4
   STA   __local_f0__2
   LDA   __local_f0__1
   SBC   g1+5
   STA   __local_f0__1
   LDA   __local_f0__0
   SBC   g1+6
   AND   #$7F
   STA   __local_f0__0
   LDA   __local_f0__6
   ORA   __local_f0__5
   ORA   __local_f0__4
   ORA   __local_f0__3
   ORA   __local_f0__2
   ORA   __local_f0__1
   ORA   __local_f0__0
   BNE   .or_true@15
   LDA   g1
   BMI   .sx_neg@55
   LDA   #$00
   BEQ   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   STA   __local_f0__2
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g1
   EOR   __zpabi_f0__p0_0
   STA   __local_f0__3
   LDA   __local_f0__2
   EOR   __zpabi_f0__p0_1
   STA   __local_f0__2
   LDA   __local_f0__1
   EOR   __zpabi_f0__p0_2
   STA   __local_f0__1
   LDA   __local_f0__0
   EOR   __zpabi_f0__p0_3
   STA   __local_f0__0
   LDA   __local_f0__3
   ORA   __local_f0__2
   ORA   __local_f0__1
   ORA   __local_f0__0
   BEQ   .or_end@16
.or_true@15:
.or_end@16:
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__8
   INY
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__7
   LDA   g0+3
   BMI   .sx_neg@57
   LDA   #$00
   BEQ   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   STA   __local_f0__1
   AND   #$7F
   CMP   g1+6
   BNE   .cmp_true@59
   LDA   __local_f0__1
   CMP   g1+5
   BNE   .cmp_true@59
   CMP   g1+4
   BNE   .cmp_true@59
   LDA   g0+3
   CMP   g1+3
   BNE   .cmp_true@59
   LDA   g0+2
   CMP   g1+2
   BNE   .cmp_true@59
   LDA   g0+1
   CMP   g1+1
   BNE   .cmp_true@59
   LDA   g0
   CMP   g1
   BNE   .cmp_true@59
   LDA   #$00
   BEQ   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   CLC
   ADC   __local_f0__8
   BCC   .carry_inc@1
   INC   __local_f0__7
.carry_inc@1:
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__7
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   BVC   .cmp_novf@62
   EOR   #$80
.cmp_novf@62:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f0__1
   LDA   #$00
   CLC
   JSR   f2@run3
   LDA   #$00
   SBC   g1+2
   LDA   #$00
   SBC   g1+3
   LDA   #$00
   SBC   g1+4
   LDA   #$00
   SBC   g1+5
   LDA   #$00
   JSR   f2@run2
   AND   #$07
   TAX
   BEQ   .shift_done@64
.shift_loop@63:
   ASL   __local_f0__1
   DEX
   BNE   .shift_loop@63
.shift_done@64:
   LDA   __local_f0__1
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$AA
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   JSR   f0
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   g0+2
   EOR   g1+2
   STA   __local_main__1
   LDA   g0+3
   EOR   g1+3
   STA   __local_main__0
   LDA   __local_main__4
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

g1:
   DC.B  $A8
   DS.B  6

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   LDA   #$40
   STA   g0
   RTS
