SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
__zpabi_f0__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f0__p1_0	EQU	$83
__zpabi_f0__p1_1	EQU	$84
__local_main__0	EQU	$85
__local_main__1	EQU	$86
__local_main__2	EQU	$87
__local_main__3	EQU	$88

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p2 locals= indirect=false in_cycle=false param_regs=,,,,,A
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals= indirect=true in_cycle=false
; def f4 params= locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; call f1 -> f2
; call f1 -> f3
; call f2 -> f3
; call main -> f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=4 at=auto def=true init=$0D,$00,$00,$00 weight=16
; runtime hargs_size=32 hargs_need=23
; @zp-link-meta-end

f4:
   SUBROUTINE

   LDA   g1+3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   CMP   g0+6
   BNE   .cmp_true@2
   CMP   g0+5
   BNE   .cmp_true@2
   CMP   g0+4
   BNE   .cmp_true@2
   LDA   g1+3
   CMP   g0+3
   BNE   .cmp_true@2
   LDA   g1+2
   CMP   g0+2
   BNE   .cmp_true@2
   LDA   g1+1
   CMP   g0+1
   BNE   .cmp_true@2
   LDA   g1
   CMP   g0
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   AND   #$01
   RTS

f3:
   SUBROUTINE

   ; prologue: 4 arg bytes, 1 local bytes, 1 callee-saved bytes
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
   SEC
   SBC   g1
   STA   $83
   LDA   g1+1
   SBC   g1+1
   STA   $82
   LDA   g1+2
   SBC   g1+2
   STA   $81
   LDA   g1+3
   SBC   g1+3
   STA   $80
   LDA   g1+3
   CMP   $80
   BNE   .cmp_true@5
   LDA   g1+2
   CMP   $81
   BNE   .cmp_true@5
   LDA   g1+1
   CMP   $82
   BNE   .cmp_true@5
   LDA   g1
   CMP   $83
   BNE   .cmp_true@5
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   $C0
   LDA   #<f4
   STA   $80
   LDA   #>f4
   STA   $81
   LDA   #<f4
   STA   DPTR
   LDA   #>f4
   STA   DPTR+1
   JSR   icall
   STA   $81
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   $80
   LDA   $C0
   EOR   $81
   STA   $83
   LDA   #$00
   EOR   $80
   STA   $82
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $81
   STA   $80
   LDA   g1
   CLC
   ADC   $83
   STA   g1
   LDA   g1+1
   ADC   $82
   STA   g1+1
   LDA   g1+2
   ADC   $81
   STA   g1+2
   LDA   g1+3
   ADC   $80
   STA   g1+3
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   LDY   #$04
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   HARGS+16
   LDA   HARGS+9
   STA   HARGS+17
   LDA   HARGS+10
   STA   HARGS+18
   LDA   HARGS+11
   STA   HARGS+19
   LDA   #$00
   STA   HARGS+20
   LDY   #$00
   STY   HARGS+21
   STA   HARGS+22

   ; epilogue
   INY
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$07
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

   LDA   g0
   EOR   g0
   STA   $86
   LDA   g0+1
   EOR   g0+1
   STA   $85
   LDA   g0+2
   EOR   g0+2
   STA   $84
   LDA   g0+3
   EOR   g0+3
   STA   $83
   LDA   g0+4
   EOR   g0+4
   STA   $82
   LDA   g0+5
   EOR   g0+5
   STA   $81
   LDA   g0+6
   EOR   g0+6
   STA   $80
   LDA   $86
   ORA   $85
   ORA   $84
   ORA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BEQ   .and_false@2
   LDA   g1
   AND   g1
   STA   $83
   LDA   g1+1
   AND   g1+1
   STA   $82
   LDA   g1+2
   AND   g1+2
   STA   $81
   LDA   g1+3
   AND   g1+3
   STA   $80
   LDA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BNE   .and_end@3
.and_false@2:
.and_end@3:
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$FF
   LDY   #$01
   STA   (SSP),Y
   LDA   #$7F
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+16
   STA   $80
   LDA   HARGS+17
   STA   $86
   LDA   HARGS+18
   STA   $85
   LDA   HARGS+19
   STA   $84
   LDA   HARGS+20
   STA   $83
   LDA   HARGS+21
   STA   $82
   LDA   #$07
   EOR   $80
   CLC
   ADC   g1
   STA   g1
   BCC   .carry_inc@0
   INC   g1+1
   BNE   .carry_inc@0
   INC   g1+2
   BNE   .carry_inc@0
   INC   g1+3
.carry_inc@0:
   LDA   #<f4
   STA   $80
   LDA   #>f4
   STA   $81
   LDA   #<f4
   STA   DPTR
   LDA   #>f4
   STA   DPTR+1
   JSR   icall
   LDA   #$00
   RTS

f1:
   SUBROUTINE

   ; prologue: 4 arg bytes, 11 local bytes, 11 callee-saved bytes
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

   LDA   g0
   STA   $82
   LDA   g0+1
   STA   $81
   LDY   #$0E
   LDA   (FP),Y
   CLC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $80
   ORA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDA   $82
   STA   HARGS
   LDA   $81
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+6
   STA   $CA
   BNE   .f1@jthread@0
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   ORA   g0+4
   ORA   g0+5
   ORA   g0+6
   BEQ   .and_false@4
.f1@jthread@0:
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   LDA   g1+2
   STA   $81
   LDA   g1+3
   LDY   #$11
   CMP   (FP),Y
   BNE   .and_false@4
   LDA   $81
   DEY
   CMP   (FP),Y
   BNE   .and_false@4
   LDA   $82
   DEY
   CMP   (FP),Y
   BNE   .and_false@4
   LDA   $83
   DEY
   CMP   (FP),Y
   BNE   .and_false@4
   LDA   #$01
   STA   $82
   LDA   #$00
   STA   $81
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   STA   $82
   LDY   #$00
   STY   $81
.and_end@5:
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   $80
   LDA   $82
   STA   $C9
   LDA   #$00
   STA   $C8
   LDA   $80
   AND   #$03
   STA   $C7
   LDA   g0
   STA   $C6
   LDA   g0+1
   STA   $C5
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   $C4
   STA   $C3
   STA   $C2
   STA   $C1
   STA   $C0
   LDA   g1
   AND   #$07
   STA   HARGS+8
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDA   g0+4
   STA   HARGS+4
   LDA   g0+5
   STA   HARGS+5
   LDA   g0+6
   STA   HARGS+6
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+7
   JSR   asr64
   LDA   HARGS+9
   STA   $86
   LDA   HARGS+10
   STA   $85
   LDA   HARGS+11
   STA   $84
   LDA   HARGS+12
   STA   $83
   LDA   HARGS+13
   STA   $82
   LDA   HARGS+14
   STA   $81
   LDA   $C0
   CMP   HARGS+15
   BNE   .thread@0
   LDA   $C1
   CMP   $81
   BNE   .thread@0
   LDA   $C2
   CMP   $82
   BNE   .thread@0
   LDA   $C3
   CMP   $83
   BNE   .thread@0
   LDA   $C4
   CMP   $84
   BNE   .thread@0
   LDA   $C5
   CMP   $85
   BNE   .thread@0
   LDA   $C6
   CMP   $86
   BEQ   .cmp_true@16
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   $80
   ASL
   STA   $85
   LDA   #$00
   STA   $86
   ROL   $86
   ASL   $85
   ROL   $86
   ASL   $85
   ROL   $86
   LDA   $85
   SEC
   SBC   $80
   STA   $84
   BCS   .carry_inc@1
   DEC   $86
.carry_inc@1:
   LDA   $86
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   HARGS+2
   STA   HARGS+3
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   LDA   g1+2
   STA   $81
   LDA   g1+3
   STA   $80
   LDA   $83
   SEC
   LDY   #$0E
   SBC   (FP),Y
   STA   HARGS+4
   LDA   $82
   INY
   SBC   (FP),Y
   STA   HARGS+5
   LDA   $81
   INY
   SBC   (FP),Y
   STA   HARGS+6
   LDA   $80
   INY
   SBC   (FP),Y
   STA   HARGS+7
   LDA   $84
   STA   HARGS
   LDA   $86
   STA   HARGS+1
   JSR   mul32
   LDA   HARGS+8
   STA   $C3
   LDA   HARGS+9
   STA   $C2
   LDA   HARGS+10
   STA   $C1
   LDA   HARGS+11
   STA   $C0
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f3
   LDA   $CA
   EOR   HARGS+16
   CLC
   ADC   g0
   STA   $86
   LDA   g0+1
   ADC   #$00
   STA   $85
   LDA   g0+2
   ADC   #$00
   STA   $84
   LDA   g0+3
   ADC   #$00
   STA   $83
   LDA   g0+4
   ADC   #$00
   STA   $82
   LDA   g0+5
   ADC   #$00
   STA   $81
   LDA   g0+6
   ADC   #$00
   AND   #$01
   EOR   #$01
   STA   $80
   LDA   $86
   STA   g0
   LDA   $85
   STA   g0+1
   LDA   $84
   STA   g0+2
   LDA   $83
   STA   g0+3
   LDA   $82
   STA   g0+4
   LDA   $81
   STA   g0+5
   LDA   $80
   SEC
   SBC   #$01
   STA   g0+6
   JSR   f2
   ORA   #$00
   BEQ   .if_else@9
   LDA   $C3
   CLC
   ADC   #$03
   STA   $83
   LDA   $C2
   ADC   #$00
   STA   $C2
   LDA   $C1
   ADC   #$00
   STA   $C1
   LDA   $C0
   ADC   #$00
   STA   $C0
   LDA   #$00
   ADC   #$00
   STA   $82
   LDA   #$00
   ADC   #$00
   STA   $81
   LDA   #$00
   ADC   #$00
   STA   $80
   LDA   $83
   STA   $86
   LDA   $C2
   STA   $85
   LDA   $C1
   STA   $84
   LDA   $C0
   STA   $83
   LDA   $80
   AND   #$01
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
   EOR   #$01
   SEC
   SBC   #$01
   STA   $87
   JMP   .if_end@8
.if_else@9:
   LDA   $C3
   EOR   #$05
   STA   $8D
   LDA   $C2
   STA   $8C
   LDA   $C1
   STA   $8B
   LDA   $C0
   STA   $8A
   LDA   #$00
   STA   $89
   LDY   #$00
   STY   $88
   STA   $87
.if_end@8:
   LDA   g0
   CLC
   ADC   $8D
   STA   $86
   LDA   g0+1
   ADC   $8C
   STA   $85
   LDA   g0+2
   ADC   $8B
   STA   $84
   LDA   g0+3
   ADC   $8A
   STA   $83
   LDA   g0+4
   ADC   $89
   STA   $82
   LDA   g0+5
   ADC   $88
   STA   $81
   LDA   g0+6
   ADC   $87
   AND   #$01
   EOR   #$01
   STA   $80
   LDA   $86
   STA   g0
   LDA   $85
   STA   g0+1
   LDA   $84
   STA   g0+2
   LDA   $83
   STA   g0+3
   LDA   $82
   STA   g0+4
   LDA   $81
   STA   g0+5
   LDA   $80
   SEC
   SBC   #$01
   STA   g0+6
   LDA   $8D
   SEC
   SBC   $C9
   LDA   $8C
   SBC   $C8
   LDA   $8B
   SBC   $C7
   LDA   $8A
   SBC   #$00
   LDA   $89
   SBC   #$00
   LDA   $88
   SBC   #$00
   LDA   $87
   SBC   #$00
   BVC   .jcmp_novf@21
   EOR   #$80
.jcmp_novf@21:
   BMI   .or_true@10
   LDA   #$00
   STA   $81
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   STA   $81
   LDA   #$00
.or_end@11:
   ORA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   HARGS+10
   STA   HARGS+11
   LDA   $81
   STA   HARGS+8
   LDA   #$00
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
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f0:
   SUBROUTINE

   LDA   __zpabi_f0__p0_0
   STA   HARGS+8
   LDA   __zpabi_f0__p0_1
   STA   HARGS+9
   LDA   __zpabi_f0__p0_2
   STA   HARGS+10
   LDA   #$00
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$28
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_2
   LDA   #$0C
   STA   __zpabi_f0__p1_0
   LDA   #$01
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p1_1
   LDA   #$58
   JSR   f0
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
   LDA   HARGS+8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   EOR   __local_main__1
   STA   HARGS+2
   LDA   HARGS+11
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
   DC.B  $33
   DS.B  6

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   LDA   #$0D
   STA   g1
   RTS
