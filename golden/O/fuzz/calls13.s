SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$28
__zpabi_f3__p0_0	EQU	$80
__zpabi_f3__p0_1	EQU	$81
__zpabi_f3__p0_2	EQU	$82
__zpabi_f3__p0_3	EQU	$83
__zpabi_f3__p0_4	EQU	$84
__zpabi_f3__p0_5	EQU	$85
__zpabi_f3__p2_0	EQU	$86
__zpabi_f3__p2_1	EQU	$87
__zpabi_f3__p2_2	EQU	$88
__zpabi_f3__p2_3	EQU	$89
__local_f3__0	EQU	$8A
__local_f3__1	EQU	$8B
__local_f3__2	EQU	$8C
__local_f3__3	EQU	$8D
__local_f3__4	EQU	$8E
__local_f3__5	EQU	$8F
__local_f3__6	EQU	$90
__local_f3__7	EQU	$91
__local_f3__8	EQU	$92
__local_f3__9	EQU	$93
__local_f3__10	EQU	$94
__local_rec__0	EQU	$96
__local_rec__1	EQU	$97
__local_rec__2	EQU	$98
__local_rec__3	EQU	$99
__local_rec__4	EQU	$9A

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p0_4,__zpabi_f3__p0_5,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p2_2,__zpabi_f3__p2_3 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__6,__local_f3__7,__local_f3__8,__local_f3__9,__local_f3__10,__local_f3__11 indirect=false in_cycle=false param_regs=,,,,,,A,X,,,,
; def f4 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f3
; call f0 -> rec
; call f2 -> f3
; call f2 -> rec
; call f3 -> rec
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$A6,$00 weight=19
; zp g1 size=2 at=auto def=true init=$AA,$00 weight=15
; runtime hargs_size=32 hargs_need=30
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

   LDY   #$05
   LDA   (FP),Y
   ASL
   ASL
   ASL
   STA   __local_rec__0
   SEC
   SBC   (FP),Y
   STA   __local_rec__4
   DEY
   LDA   (FP),Y
   BMI   .jcmp_neg@0
   DEY
   ORA   (FP),Y
   BNE   .if_end@0
.jcmp_neg@0:
   LDY   #$05
   LDA   (FP),Y
   STA   __local_rec__0
   INY
   LDA   (FP),Y
   LDX   #$00
   LDA   __local_rec__0

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
   STA   __local_rec__2
   INY
   LDA   (FP),Y
   ROL
   STA   __local_rec__3
   LDA   __local_rec__2
   CLC
   DEY
   ADC   (FP),Y
   STA   __local_rec__1
   LDA   __local_rec__3
   INY
   ADC   (FP),Y
   STA   __local_rec__0
   LDA   __local_rec__1
   PHA
   LDA   __local_rec__0
   PHA
   LDA   __local_rec__1
   CLC
   LDY   #$03
   ADC   (FP),Y
   STA   __local_rec__1
   LDA   __local_rec__0
   INY
   ADC   (FP),Y
   STA   __local_rec__0
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   STA   __local_rec__3
   INY
   LDA   (FP),Y
   SBC   #$00
   STA   __local_rec__2
   LDA   __local_rec__0
   AND   #$03
   STA   __local_rec__0
   LDA   __local_rec__4
   PHA
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   __local_rec__3
   LDY   #$01
   STA   (SSP),Y
   LDA   __local_rec__2
   INY
   STA   (SSP),Y
   LDA   __local_rec__1
   INY
   STA   (SSP),Y
   LDA   __local_rec__0
   INY
   STA   (SSP),Y
   JSR   rec
   STA   __local_rec__4
   STX   __local_rec__3
   PLA
   STA   __local_rec__2
   PLA
   PLA
   STA   __local_rec__0
   LDA   __local_rec__4
   CLC
   ADC   __local_rec__2
   STA   __local_rec__2
   BCC   .carry_inc@0
   INC   __local_rec__3
.carry_inc@0:
   LDA   __local_rec__0
   AND   #$0F
   CLC
   ADC   __local_rec__2
   STA   __local_rec__0
   LDA   __local_rec__3
   ADC   #$00
   AND   #$03
   TAX
   LDA   __local_rec__0

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

f4:
   SUBROUTINE

   ; prologue: 16 arg bytes, 0 local bytes
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

   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   DEY
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

   LDA   #$FF
   CLC
   ADC   __zpabi_f3__p0_0
   STA   __local_f3__10
   LDA   #$7F
   ADC   __zpabi_f3__p0_1
   STA   __local_f3__9
   LDA   #$00
   ADC   __zpabi_f3__p0_2
   STA   __local_f3__8
   LDA   #$00
   ADC   __zpabi_f3__p0_3
   STA   __local_f3__7
   LDA   #$00
   ADC   __zpabi_f3__p0_4
   STA   __local_f3__6
   LDA   #$00
   ADC   __zpabi_f3__p0_5
   STA   __local_f3__5
   LDA   g1
   ORA   g1+1
   BEQ   .lnot_true@1
   LDA   #$00
   BEQ   .lnot_end@2
.lnot_true@1:
   LDA   #$01
.lnot_end@2:
   STA   __local_f3__4
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_f3__3
   STA   __local_f3__2
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   __local_f3__5
   CMP   __local_f3__0
   BNE   .cmp_true@5
   LDA   __local_f3__6
   CMP   __local_f3__1
   BNE   .cmp_true@5
   LDA   __local_f3__7
   CMP   __local_f3__2
   BNE   .cmp_true@5
   LDA   __local_f3__8
   CMP   __local_f3__3
   BNE   .cmp_true@5
   LDA   __local_f3__9
   BNE   .cmp_true@5
   LDA   __local_f3__10
   CMP   __local_f3__4
   BNE   .cmp_true@5
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   __local_f3__2
   LDA   __zpabi_f3__p2_3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   HARGS+4
   STA   HARGS+5
   LDA   __zpabi_f3__p0_0
   ORA   #$01
   STA   HARGS+8
   LDA   __zpabi_f3__p0_1
   STA   HARGS+9
   LDA   __zpabi_f3__p0_2
   STA   HARGS+10
   LDA   __zpabi_f3__p0_3
   STA   HARGS+11
   LDA   __zpabi_f3__p0_4
   STA   HARGS+12
   LDA   __zpabi_f3__p0_5
   STA   HARGS+13
   LDA   __zpabi_f3__p2_0
   STA   HARGS
   LDA   __zpabi_f3__p2_1
   STA   HARGS+1
   LDA   __zpabi_f3__p2_2
   STA   HARGS+2
   LDA   __zpabi_f3__p2_3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   STA   __local_f3__1
   LDA   HARGS+17
   STA   __local_f3__0
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
   LDA   __local_f3__1
   INY
   STA   (SSP),Y
   LDA   __local_f3__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __local_f3__2
   CMP   __local_f3__2
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   EOR   #$FF
   STA   __local_f3__1
   LDA   #$00
   EOR   #$FF
   STA   __local_f3__0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__1
   STA   HARGS+8
   LDA   __local_f3__0
   STA   HARGS+9
   RTS

f2:
   SUBROUTINE

   ; prologue: 3 arg bytes, 7 local bytes, 7 callee-saved bytes
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

   LDY   #$0C
   LDA   (FP),Y
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   $C0
   LDY   #$0A
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   g0
   STA   HARGS+2
   LDA   g0+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $8B
   LDA   HARGS+5
   STA   $8A
   LDA   $8B
   SEC
   LDY   #$0C
   SBC   (FP),Y
   LDA   $8A
   SBC   $C0
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   STA   $C6
   LDA   #$FF
   ORA   g1
   STA   $8D
   LDA   #$7F
   ORA   g1+1
   STA   $8C
   LDY   #$0C
   LDA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   STA   $8B
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   $8B
.or_end@2:
   LDA   $8D
   CLC
   SBC   $8B
   LDA   $8C
   SBC   #$00
   LDA   #$00
   ROL
   STA   $C5
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   $C4
   STA   $C3
   LDY   #$0B
   LDA   (FP),Y
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   $C2
   STA   $C1
   LDA   $C6
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   $8F
   STA   $8E
   STA   $8D
   STA   $8C
   STA   $8B
   LDA   $C6
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   $C0
   LDA   #$FF
   SEC
   SBC   $C6
   STA   $8A
   LDA   #$00
   SBC   $C0
   TAX
   LDA   $C6
   STA   __zpabi_f3__p0_0
   LDA   $8F
   STA   __zpabi_f3__p0_1
   LDA   $8E
   STA   __zpabi_f3__p0_2
   LDA   $8D
   STA   __zpabi_f3__p0_3
   LDA   $8C
   STA   __zpabi_f3__p0_4
   LDA   $8B
   STA   __zpabi_f3__p0_5
   LDA   #$07
   STA   __zpabi_f3__p2_0
   LDA   #$00
   STA   __zpabi_f3__p2_1
   STA   __zpabi_f3__p2_2
   STA   __zpabi_f3__p2_3
   LDA   $8A
   JSR   f3
   LDA   HARGS+8
   STA   $8D
   LDA   HARGS+9
   STA   $8C
   LDA   HARGS+10
   STA   $8B
   LDA   HARGS+11
   STA   $8A
   LDY   #$0A
   LDA   (FP),Y
   EOR   $8D
   STA   $99
   INY
   LDA   (FP),Y
   EOR   $8C
   STA   $98
   LDA   $C2
   EOR   $8B
   STA   $97
   LDA   $C1
   EOR   $8A
   STA   $96
   LDA   g0
   CLC
   ADC   $99
   STA   g0
   LDA   g0+1
   ADC   $98
   STA   g0+1
   LDA   #<f4
   STA   $94
   LDA   #>f4
   STA   $95
   LDA   $C6
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   $93
   STA   $92
   STA   $91
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   $90
   STA   $8F
   LDA   $C5
   SEC
   SBC   $99
   LDA   #$00
   SBC   $98
   LDA   $C4
   SBC   $97
   LDA   $C3
   SBC   $96
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $8E
   LDA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   STA   $8D
   STA   $8C
   LDA   $96
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   $8B
   STA   $8A
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C6
   LDY   #$01
   STA   (SSP),Y
   LDA   $93
   INY
   STA   (SSP),Y
   LDA   $92
   INY
   STA   (SSP),Y
   LDA   $91
   INY
   STA   (SSP),Y
   LDA   $90
   INY
   STA   (SSP),Y
   LDA   $8F
   INY
   STA   (SSP),Y
   LDA   $8E
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $8D
   INY
   STA   (SSP),Y
   LDA   $8C
   INY
   STA   (SSP),Y
   LDA   $99
   INY
   STA   (SSP),Y
   LDA   $98
   INY
   STA   (SSP),Y
   LDA   $97
   INY
   STA   (SSP),Y
   LDA   $96
   INY
   STA   (SSP),Y
   LDA   $8B
   INY
   STA   (SSP),Y
   LDA   $8A
   INY
   STA   (SSP),Y
   LDA   $94
   STA   DPTR
   LDA   $95
   STA   DPTR+1
   JSR   icall
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
   LDA   #$E8
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   $C6
   SEC
   SBC   #$FF
   STA   $8B
   LDA   $C0
   SBC   #$7F
   ORA   $8B
   BEQ   .cmp_end@34
   LDA   #$01
.cmp_end@34:

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
   RTS

f1:
   SUBROUTINE

   ; prologue: 9 arg bytes, 4 local bytes, 4 callee-saved bytes
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

   SEC
   LDA   #$00
   SBC   g0
   STA   $C3
   LDA   #$00
   SBC   g0+1
   STA   $C2
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   $C1
   STA   $C0
   LDY   #$0E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $81
   LDY   #$0D
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@37
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   CLC
   ADC   $82
   STA   $80
   BCC   .carry_inc@1
   INC   $81
.carry_inc@1:
   LDY   #$0E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f4
   STA   $88
   LDA   #>f4
   STA   $89
   LDA   $C0
   BMI   .sx_neg@40
   LDA   #$00
   BEQ   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   STA   $81
   STA   $80
   LDY   #$08
   LDA   (FP),Y
   SEC
   SBC   $C3
   INY
   LDA   (FP),Y
   SBC   $C2
   INY
   LDA   (FP),Y
   SBC   $C1
   INY
   LDA   (FP),Y
   SBC   $C0
   INY
   LDA   (FP),Y
   SBC   $81
   INY
   LDA   (FP),Y
   SBC   $80
   LDA   #$00
   ROL
   STA   $87
   LDA   #$00
   BPL   .sx_done@43
   LDA   #$FF
.sx_done@43:
   STA   $86
   STA   $85
   STA   $84
   STA   $83
   LDY   #$07
   LDA   (FP),Y
   BMI   .sx_neg@44
   LDA   #$00
   BEQ   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   STA   $82
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $87
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
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
   INY
   LDA   (FP),Y
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
   LDA   #$E8
   INY
   STA   (SSP),Y
   LDA   #$03
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
   LDA   $88
   STA   DPTR
   LDA   $89
   STA   DPTR+1
   JSR   icall
   STA   $81
   STX   $80
   LDA   $C3
   EOR   $81
   STA   $86
   LDA   $C2
   EOR   $80
   STA   $85
   LDA   $C1
   STA   $84
   LDA   $C0
   STA   $83
   LDY   #$07
   LDA   (FP),Y
   BMI   .sx_neg@46
   LDA   #$00
   BEQ   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   STA   $88
   LDY   #$07
   LDA   (FP),Y
   BMI   .sx_neg@48
   LDA   #$00
   BEQ   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   STA   $82
   STA   $81
   STA   $80
   LDA   $86
   SEC
   LDY   #$07
   SBC   (FP),Y
   STA   $87
   LDA   $85
   SBC   $82
   STA   $86
   LDA   $84
   SBC   $81
   STA   $85
   LDA   $83
   SBC   $80
   STA   $84
   LDA   (FP),Y
   EOR   #$FF
   STA   $83
   LDA   $88
   EOR   #$FF
   STA   $82
   BMI   .sx_neg@50
   LDA   #$00
   BEQ   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
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
   BVC   .cmp_novf@52
   EOR   #$80
.cmp_novf@52:
   ASL
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
   PLA
   TAX
   PLA
   RTS

f0:
   SUBROUTINE

   ; prologue: 1 arg bytes, 12 local bytes, 12 callee-saved bytes
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

   LDY   #$0F
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   g0
   STA   HARGS+2
   LDA   g0+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $8B
   LDA   #$7F
   CMP   HARGS+5
   BNE   .cmp_true@53
   LDA   #$FF
   CMP   $8B
   BNE   .cmp_true@53
   LDA   #$00
   BEQ   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   STA   $8B
   LDY   #$00
   STY   $8A
   ORA   #$00
   BNE   .or_true@3
   LDY   #$0F
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+1
   LDA   (FP),Y
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+3
   STA   $8A
   BNE   .or_true@3
   LDA   #$00
   STA   $CB
   STA   $CA
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   $CB
   LDA   #$00
   STA   $CA
.or_end@4:
   ORA   #$00
   BPL   .sx_done@57
   LDA   #$FF
.sx_done@57:
   STA   $C7
   STA   $C6
   STA   $C5
   STA   $C4
   LDY   #$0F
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+1
   LDA   (FP),Y
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+2
   STA   $8B
   LDA   g0
   LDY   #$0F
   AND   (FP),Y
   STA   $8A
   AND   $8B
   STA   $C3
   LDA   g0
   STA   $C9
   LDA   #<f4
   STA   $C1
   LDA   #>f4
   STA   $C2
   LDA   $C9
   BMI   .sx_neg@58
   LDA   #$00
   BEQ   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   STA   $C0
   BMI   .sx_neg@60
   LDA   #$00
   BEQ   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   STA   HARGS+2
   STA   HARGS+3
   STA   HARGS+4
   STA   HARGS+5
   LDA   $CB
   ORA   #$01
   STA   HARGS+8
   LDA   $CA
   STA   HARGS+9
   LDA   $C7
   STA   HARGS+10
   LDA   $C6
   STA   HARGS+11
   LDA   $C5
   STA   HARGS+12
   LDA   $C4
   STA   HARGS+13
   LDA   $C9
   STA   HARGS
   LDA   $C0
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   STA   $94
   LDA   HARGS+25
   STA   $93
   LDA   HARGS+26
   STA   $92
   LDA   HARGS+27
   STA   $91
   LDA   HARGS+28
   STA   $90
   LDA   HARGS+29
   STA   $8F
   LDA   g0
   SEC
   SBC   $C9
   LDA   g0+1
   SBC   $C0
   BVC   .cmp_novf@62
   EOR   #$80
.cmp_novf@62:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $8E
   LDA   #$00
   BPL   .sx_done@64
   LDA   #$FF
.sx_done@64:
   STA   $8D
   STA   $8C
   STA   $8B
   STA   $8A
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $94
   LDY   #$01
   STA   (SSP),Y
   LDA   $93
   INY
   STA   (SSP),Y
   LDA   $92
   INY
   STA   (SSP),Y
   LDA   $91
   INY
   STA   (SSP),Y
   LDA   $90
   INY
   STA   (SSP),Y
   LDA   $8F
   INY
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   $8E
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $8D
   INY
   STA   (SSP),Y
   LDA   $8C
   INY
   STA   (SSP),Y
   LDA   $8B
   INY
   STA   (SSP),Y
   LDA   $8A
   INY
   STA   (SSP),Y
   LDA   $C1
   STA   DPTR
   LDA   $C2
   STA   DPTR+1
   JSR   icall
   STA   $8B
   STX   $8A
   LDA   $C3
   EOR   $8B
   STA   $C5
   LDA   #$00
   EOR   $8A
   STA   $C4
   LDA   #$00
   STA   $C3
   STA   $C2
   LDA   g1
   CLC
   ADC   $C5
   STA   g1
   LDA   g1+1
   ADC   $C4
   STA   g1+1
   LDA   $C9
   BMI   .sx_neg@65
   LDA   #$00
   BEQ   .sx_done@66
.sx_neg@65:
   LDA   #$FF
.sx_done@66:
   STA   $C8
   STA   $C7
   STA   $C6
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
   BVC   .jcmp_novf@67
   EOR   #$80
.jcmp_novf@67:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   g1
   STA   $8B
   LDA   g1+1
   STA   $8A
   LDA   $8B
   ORA   $C5
   STA   __zpabi_f3__p0_0
   LDA   $8A
   ORA   $C4
   STA   __zpabi_f3__p0_1
   LDA   $C3
   STA   __zpabi_f3__p0_2
   LDA   $C2
   STA   __zpabi_f3__p0_3
   LDA   #$00
   STA   __zpabi_f3__p0_4
   STA   __zpabi_f3__p0_5
   LDA   $C9
   STA   __zpabi_f3__p2_0
   LDA   $C8
   STA   __zpabi_f3__p2_1
   LDA   $C7
   STA   __zpabi_f3__p2_2
   LDA   $C6
   STA   __zpabi_f3__p2_3
   LDX   g0+1
   LDA   g0
   JSR   f3
   LDA   HARGS+8
   STA   $8D
   LDA   HARGS+9
   STA   $8C
   LDA   HARGS+10
   STA   $8F
   LDA   HARGS+11
   STA   $8E
   LDA   $C0
   BMI   .sx_neg@68
   LDA   #$00
   BEQ   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   STA   $8B
   STA   $8A
   LDA   $8D
   CLC
   ADC   $C1
   STA   $8D
   LDA   $8C
   ADC   $C0
   STA   $8C
   LDA   $8F
   ADC   $8B
   STA   $8B
   LDA   $8E
   ADC   $8A
   STA   $8A
   LDA   $C5
   CLC
   ADC   $8D
   STA   $C5
   LDA   $C4
   ADC   $8C
   STA   $C4
   LDA   $C3
   ADC   $8B
   STA   $C3
   LDA   $C2
   ADC   $8A
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
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
   LDA   $CB
   INY
   STA   (SSP),Y
   LDA   $CA
   INY
   STA   (SSP),Y
   JSR   rec
   LDX   $CA
   LDA   $CB

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
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
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

   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$17
   DEY
   STA   (SSP),Y
   JSR   f0
   STA   $89
   STX   $88
   TXA
   BMI   .sx_neg@70
   LDA   #$00
   BEQ   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   STA   $87
   STA   $86
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   BMI   .sx_neg@72
   LDA   #$00
   BEQ   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   STA   $85
   STA   $84
   LDA   g1
   STA   $81
   LDA   g1+1
   STA   $80
   LDA   $83
   EOR   $81
   STA   $83
   LDA   $82
   EOR   $80
   STA   $82
   LDA   $85
   STA   $81
   STA   $80
   LDA   $89
   EOR   $83
   STA   $C1
   TXA
   EOR   $82
   STA   $C0
   LDA   $87
   EOR   $81
   STA   HARGS+2
   LDA   $86
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g1+1
   LDA   #$A6
   STA   g0
   LDA   #$AA
   STA   g1
   RTS
