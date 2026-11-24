SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
__zpabi_f2__p1_0	EQU	$80
__zpabi_f2__p1_1	EQU	$81
__zpabi_f2__pp_0	EQU	$82
__zpabi_f2__pp_1	EQU	$83
__local_f1__0	EQU	$84
__local_f1__1	EQU	$85
__local_f1__2	EQU	$86
__local_f1__7	EQU	$8B
__local_f1__box2	EQU	$8C
__local_f2__p0_1	EQU	$8E
__local_f2__p0_0	EQU	$8F
__local_f2__0	EQU	$90
__local_f2__1	EQU	$91
__local_f2__3	EQU	$93
__local_f2__4	EQU	$94

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params= locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__box2,__local_f1__9 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__p0_1,__local_f2__p0_0,__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__8 indirect=false in_cycle=false param_regs=A,X,,,,
; def f3 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f2
; call f0 -> rec
; call f1 -> f2
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
; zp g0 size=1 at=auto def=true init=$23 weight=13
; zp g1 size=1 at=auto def=true init=$99 weight=7
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

   ; prologue: 13 arg bytes, 2 local bytes, 2 callee-saved bytes
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

   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@1
   LDY   #$0E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@1
   LDA   #$01
   STA   $C1
   LDA   #$00
   STA   $C0
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   STA   $C1
   STA   $C0
.and_end@2:
   LDA   g1
   STA   $83
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   $82
   LDA   g0
   BMI   .thread@0
   LDA   #$00
   BEQ   .vthread@0
.thread@0:
   LDA   #$00
.vthread@0:
   ORA   $83
   STA   $81
   LDA   $82
   AND   #$01
   STA   $80
   LDA   $81
   STA   $83
   LDA   $80
   EOR   #$01
   SEC
   SBC   #$01
   STA   $82
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   LDA   $81
   CLC
   LDY   #$07
   ADC   (FP),Y
   STA   $81
   LDA   $80
   INY
   ADC   (FP),Y
   STA   $80
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$03
   CLC
   ADC   $83
   STA   $81
   LDA   #$00
   ADC   $82
   STA   $80
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
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
   CLC
   ADC   $C1
   STA   $84
   TXA
   ADC   $C0
   STA   $83
   LDY   #$05
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   STA   $82
   LDA   g0
   STA   $81
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   CMP   $83
   BNE   .cmp_true@11
   LDA   $81
   CMP   $84
   BNE   .cmp_true@11
   LDA   #$00
   BEQ   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   STA   $80
   LDA   $82
   SEC
   SBC   $80
   STA   $81
   LDA   #$00
   SBC   #$00
   STA   $80
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
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
   CLC
   LDA   FP
   ADC   #$11
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

f2:
   SUBROUTINE

   STA   __local_f2__p0_0
   STX   __local_f2__p0_1
   LDA   __zpabi_f2__p1_1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f2__4
   STA   __local_f2__3
   SEC
   LDA   #$00
   SBC   __local_f2__p0_0
   STA   __local_f2__1
   LDA   #$00
   SBC   __local_f2__p0_1
   STA   __local_f2__0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f2__p0_0
   STA   __local_f2__p0_1
   LDA   __zpabi_f2__p1_1
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_f2__1
   STA   HARGS
   LDA   __local_f2__0
   STA   HARGS+1
   LDA   __local_f2__p0_0
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f2__p1_0
   STA   HARGS+4
   LDA   __zpabi_f2__p1_1
   STA   HARGS+5
   JSR   mul32
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   LDA   __local_f2__1
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f2__0
   STA   HARGS+5
   LDA   __local_f2__p0_0
   STA   HARGS+6
   LDA   __local_f2__p0_1
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+14
   AND   #$01
   STA   __local_f2__p0_1
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__0
   LDA   __zpabi_f2__p1_0
   ORA   __zpabi_f2__p1_1
   BNE   .or_true@3
   LDA   HARGS+12
   ORA   HARGS+13
   ORA   __local_f2__p0_1
   BNE   .or_true@3
   LDA   #$00
   STA   __local_f2__p0_0
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   __local_f2__p0_0
.or_end@4:
   LDA   __local_f2__1
   CLC
   ADC   __local_f2__p0_0
   STA   __local_f2__p0_0
   LDA   __local_f2__0
   ADC   #$00
   LDY   #$01
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__p0_0
   DEY
   STA   (__zpabi_f2__pp_0),Y
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__0
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   CMP   __local_f2__3
   BNE   .cmp_true@24
   CMP   __local_f2__4
   BNE   .cmp_true@24
   LDA   __local_f2__0
   CMP   __zpabi_f2__p1_1
   BNE   .cmp_true@24
   LDA   __local_f2__1
   CMP   __zpabi_f2__p1_0
   BNE   .cmp_true@24
   LDA   #$00
   BEQ   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   CLC
   ADC   __zpabi_f2__p1_0
   STA   __local_f2__0
   LDA   #$00
   ADC   __zpabi_f2__p1_1
   STA   __local_f2__p0_0
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDX   __local_f2__0
   STX   HARGS+8
   LDX   __local_f2__p0_0
   STX   HARGS+9
   AND   #$01
   STA   HARGS+10
   RTS

f1:
   SUBROUTINE

   LDA   g1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   HARGS+1
   LDA   g1
   STA   HARGS
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .or_true@5
   LDA   g0
   BNE   .or_true@5
   LDA   #$00
   STA   __local_f1__7
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   STA   __local_f1__7
.or_end@6:
   LDA   g0
   AND   #$07
   TAX
   LDA   __local_f1__7
   STA   __local_f1__0
   CPX   #$00
   BEQ   .shift_done@34
.shift_loop@33:
   LSR   __local_f1__0
   DEX
   BNE   .shift_loop@33
.shift_done@34:
   ORA   __local_f1__0
   STA   __zpabi_f2__p1_0
   LDA   __local_f1__7
   STA   __local_f1__box2
   LDA   #$00
   STA   __local_f1__box2+1
   LDA   g0
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   __local_f1__1
   LDA   __zpabi_f2__p1_0
   ORA   g0
   STA   __local_f1__2
   LDX   __local_f1__1
   LDA   #$00
   STA   __zpabi_f2__p1_1
   LDA   #<__local_f1__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f1__box2
   STA   __zpabi_f2__pp_1
   LDA   __local_f1__2
   JSR   f2
   LDA   g1
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_f1__0
   LDA   #$03
   EOR   g1
   STA   __local_f1__1
   LDA   #$00
   EOR   __local_f1__0
   ORA   __local_f1__1
   BEQ   .and_false@7
   LDA   g1
   BEQ   .and_false@7
   LDA   #$01
   LDX   #$00
   RTS
.and_false@7:
   LDA   #$00
   LDX   #$00
   RTS

f0:
   SUBROUTINE

   ; prologue: 0 arg bytes, 15 local bytes, 11 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$10
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

   LDA   g0
   STA   $CA
   STA   $C0
   BMI   .sx_neg@41
   LDA   #$00
   BEQ   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   STA   $C1
   LDA   $CA
   SEC
   SBC   $CA
   STA   $87
   LDA   #$00
   SBC   #$00
   STA   $86
   LDA   g0
   STA   $85
   BMI   .sx_neg@43
   LDA   #$00
   BEQ   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   STA   $84
   LDA   $85
   ORA   $CA
   ORA   $87
   STA   $C9
   LDA   $86
   ORA   $84
   STA   $C8
   BMI   .sx_neg@45
   LDA   #$00
   BEQ   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   $C7
   STA   $C6
   LDA   g1
   STA   $C5
   BMI   .sx_neg@47
   LDA   #$00
   BEQ   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   STA   $C4
   STA   $C3
   STA   $C2
   LDA   $CA
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<f3
   STA   $8B
   LDA   #>f3
   STA   $8C
   LDA   g0
   STA   $8A
   BMI   .sx_neg@49
   LDA   #$00
   BEQ   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   STA   $89
   LDA   g0
   STA   $87
   BMI   .sx_neg@51
   LDA   #$00
   BEQ   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   STA   $86
   BMI   .sx_neg@53
   LDA   #$00
   BEQ   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   STA   $85
   STA   $84
   LDA   $C9
   SEC
   SBC   $87
   LDA   $C8
   SBC   $86
   LDA   $C7
   SBC   $85
   LDA   $C6
   SBC   $84
   BVC   .cmp_novf@55
   EOR   #$80
.cmp_novf@55:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $85
   LDA   #$00
   BPL   .sx_done@57
   LDA   #$FF
.sx_done@57:
   STA   $84
   LDA   $85
   STA   $88
   LDA   #$00
   STA   $87
   LDA   $84
   AND   #$01
   STA   $86
   CLC
   LDA   FP
   ADC   #$0C
   STA   $84
   LDA   FP+1
   ADC   #$00
   STA   $85
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
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   $86
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
   LDA   $CA
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $85
   INY
   STA   (SSP),Y
   LDA   $8B
   STA   DPTR
   LDA   $8C
   STA   DPTR+1
   JSR   icall
   LDA   $CA
   EOR   HARGS+8
   STA   $85
   LDY   #$0C
   LDA   (FP),Y
   CLC
   ADC   $85
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   AND   #$07
   TAX
   CPX   #$00
   BEQ   .shift_done@59
.shift_loop@58:
   LSR   $C1
   ROR   $C0
   DEX
   BNE   .shift_loop@58
.shift_done@59:
   CLC
   LDA   FP
   ADC   #$0E
   STA   $84
   LDA   FP+1
   ADC   #$00
   STA   $85
   LDA   $C5
   STA   __zpabi_f2__p1_0
   LDA   $C4
   STA   __zpabi_f2__p1_1
   LDA   $84
   STA   __zpabi_f2__pp_0
   LDA   $85
   STA   __zpabi_f2__pp_1
   LDX   $C1
   LDA   $C0
   JSR   f2
   LDA   HARGS+8
   STA   $86
   LDA   HARGS+9
   STA   $85
   LDA   HARGS+10
   STA   $84
   LDA   $C5
   EOR   $86
   STA   $8B
   LDA   $C4
   EOR   $85
   STA   $8A
   LDA   $C3
   EOR   $84
   STA   $89
   LDA   $C2
   STA   $88
   LDY   #$0E
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   STA   $86
   BMI   .sx_neg@60
   LDA   #$00
   BEQ   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   STA   $85
   STA   $84
   LDA   $8B
   CLC
   ADC   $87
   STA   $C3
   LDA   $8A
   ADC   $86
   STA   $C2
   LDA   $89
   ADC   $85
   STA   $C1
   LDA   $88
   ADC   $84
   STA   $C0
   LDA   g0
   CLC
   ADC   $C3
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
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   $C9
   ORA   #$01
   STA   HARGS+4
   LDA   $C8
   STA   HARGS+5
   LDA   $C7
   STA   HARGS+6
   LDA   $C6
   STA   HARGS+7
   LDA   #$03
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $87
   LDA   HARGS+9
   STA   $86
   LDA   HARGS+10
   STA   $85
   LDA   HARGS+11
   STA   $84
   LDA   $C3
   SEC
   SBC   $87
   LDA   $C2
   SBC   $86
   LDA   $C1
   SBC   $85
   LDA   $C0
   SBC   $84
   BVC   .cmp_novf@62
   EOR   #$80
.cmp_novf@62:
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
   INY
   LDA   (FP),Y
   STA   $CA
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$10
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
   STA   $89
   STX   $88
   LDA   g0
   STA   $87
   BMI   .sx_neg@63
   LDA   #$00
   BEQ   .sx_done@64
.sx_neg@63:
   LDA   #$FF
.sx_done@64:
   STA   $86
   STA   $85
   STA   $84
   LDA   g1
   STA   $83
   BMI   .sx_neg@65
   LDA   #$00
   BEQ   .sx_done@66
.sx_neg@65:
   LDA   #$FF
.sx_done@66:
   STA   $82
   STA   $81
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
   LDA   $89
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

__zp_init:
   SUBROUTINE

   LDA   #$23
   STA   g0
   LDA   #$99
   STA   g1
   RTS
