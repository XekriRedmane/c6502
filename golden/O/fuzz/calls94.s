SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f3__0	EQU	$80
__zpabi_f0__p1_0	EQU	$80
__local_f3__1	EQU	$81
__zpabi_f0__p1_1	EQU	$81
__zpabi_f1__p1_0	EQU	$82
__zpabi_f1__p1_1	EQU	$83
__zpabi_f1__p2	EQU	$84
__zpabi_f1__p3_0	EQU	$85
__zpabi_f1__p3_1	EQU	$86
__zpabi_f1__p3_2	EQU	$87
__zpabi_f1__p3_3	EQU	$88
__local_main__0	EQU	$89
__local_main__1	EQU	$8A
__local_main__4	EQU	$8D
__local_main__5	EQU	$8E
__local_f0__0	EQU	$8F
__local_f0__p0_0	EQU	$90
__local_f0__p0_1	EQU	$91
__local_f0__1	EQU	$92
__local_f0__5	EQU	$96
__local_f0__6	EQU	$97
__local_f0__7	EQU	$98
__local_f1__0	EQU	$99
__local_f1__1	EQU	$9A

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1 locals=__local_f0__0,__local_f0__p0_0,__local_f0__p0_1,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7 indirect=false in_cycle=false param_regs=A,X,,
; def f1 params=__zpabi_f1__p0,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2,__zpabi_f1__p3_0,__zpabi_f1__p3_1,__zpabi_f1__p3_2,__zpabi_f1__p3_3 locals=__local_f1__0,__local_f1__1 indirect=false in_cycle=false param_regs=A,,,,,,,
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals=__local_f3__0,__local_f3__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; call f0 -> f1
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$4D,$00 weight=8
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f3:
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

   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f3__0
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_f3__1
   CLC
   BCC   .carry_inc@0
   INC   __local_f3__1
.carry_inc@0:
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f3__0
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f3__1
   LDY   #$01
   STA   (DPTR),Y
   LDA   g0
   LDX   g0+1

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
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

   ; prologue: 5 arg bytes, 6 local bytes, 4 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
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
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $80
   SEC
   LDA   #$00
   SBC   $80
   STA   $C3
   LDA   #$00
   SBC   #$00
   STA   $C2
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@2
   LDY   #$09
   LDA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   STA   $81
   LDA   #$00
   STA   $80
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   $81
   LDY   #$00
   STY   $80
.and_end@3:
   ORA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   HARGS+2
   STA   HARGS+3
   LDY   #$0A
   LDA   (FP),Y
   ORA   #$01
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
   LDA   $81
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod32
   LDA   HARGS+12
   STA   $C1
   LDA   HARGS+13
   STA   $C0
   LDA   $C3
   LDY   #$05
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   #<f3
   STA   $86
   LDA   #>f3
   STA   $87
   LDY   #$09
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   $85
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   $84
   STA   $83
   LDA   #$64
   LDY   #$09
   AND   (FP),Y
   STA   $82
   CLC
   LDA   FP
   ADC   #$05
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $85
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $86
   STA   DPTR
   LDA   $87
   STA   DPTR+1
   JSR   icall
   LDA   $C1
   ORA   $C0
   BEQ   .and_false@4
   LDY   #$0A
   LDA   (FP),Y
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   CPX   #$00
   BEQ   .shift_done@8
.shift_loop@7:
   LSR   $C1
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   LDA   $C1
   BEQ   .and_false@4
   LDA   #$01
   LDX   #$00
   BEQ   .and_end@5
.and_false@4:
   LDA   #$00
   LDX   #$00
.and_end@5:

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
   ADC   #$0D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
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

   LDA   __zpabi_f1__p2
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_f1__0
   LDA   #$00
   SEC
   SBC   __zpabi_f1__p2
   LDA   #$00
   SBC   __local_f1__0
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f1__1
   LDA   __zpabi_f1__p1_0
   ORA   #$01
   STA   HARGS+2
   LDA   __zpabi_f1__p1_1
   STA   HARGS+3
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDX   HARGS+5
   LDA   __local_f1__1
   ORA   HARGS+4
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   STX   __local_f0__p0_1
   CPX   __local_f0__p0_1
   BNE   .thread@0
   CMP   __local_f0__p0_0
   BEQ   .cmp_true@12
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   __zpabi_f1__p2
   LDA   g0
   ORA   g0+1
   BNE   .or_true@6
   LDA   __local_f0__p0_0
   ORA   __local_f0__p0_1
   BNE   .or_true@6
   LDA   #$00
   STA   __local_f0__1
   JMP   .vthread@0
.or_true@6:
   LDA   #$01
   STA   __local_f0__1
   LDA   #$00
   BNE   .cmp_true@15
.vthread@0:
   LDA   __zpabi_f1__p2
   CMP   __local_f0__1
   BNE   .cmp_true@15
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   STA   __zpabi_f1__p2
   EOR   #$FF
   STA   __local_f0__0
   LDA   #$01
   EOR   __zpabi_f0__p1_0
   STA   __local_f0__7
   LDA   #$00
   EOR   __zpabi_f0__p1_1
   STA   __local_f0__6
   LDA   g0
   SEC
   SBC   __local_f0__0
   LDA   g0+1
   SBC   #$00
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f0__5
   LDA   __local_f0__7
   CLC
   ADC   #$64
   STA   __zpabi_f1__p1_0
   LDA   __local_f0__6
   ADC   #$00
   STA   __zpabi_f1__p1_1
   LDA   __zpabi_f1__p2
   EOR   __local_f0__p0_0
   STA   __zpabi_f1__p2
   LDA   g0+1
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __zpabi_f1__p3_2
   STA   __zpabi_f1__p3_3
   LDA   g0
   STA   __zpabi_f1__p3_0
   LDA   g0+1
   STA   __zpabi_f1__p3_1
   LDA   __local_f0__5
   JSR   f1
   STA   __local_f0__0
   STX   __local_f0__1
   LDA   __local_f0__7
   EOR   __local_f0__0
   STA   __local_f0__0
   LDA   __local_f0__6
   EOR   __local_f0__1
   STA   __zpabi_f1__p2
   LDA   __local_f0__0
   LSR
   LDA   #$00
   ROR
   STA   __local_f0__1
   LDA   #$00
   SEC
   SBC   __local_f0__0
   STA   __local_f0__0
   LDA   __local_f0__1
   SBC   __zpabi_f1__p2
   STA   __local_f0__1
   LDX   #$03
.shift_loop@23:
   LDA   __local_f0__p0_1
   ASL
   ROR   __local_f0__p0_1
   ROR   __local_f0__p0_0
   DEX
   BNE   .shift_loop@23
   LDA   __local_f0__0
   AND   __local_f0__p0_0
   STA   __local_f0__0
   LDA   __local_f0__1
   AND   __local_f0__p0_1
   TAX
   LDA   __local_f0__0
   RTS

main:
   SUBROUTINE

   LDA   #$09
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   LDX   #$00
   LDA   #$41
   JSR   f0
   STA   __local_main__5
   STX   __local_main__4
   LDA   g0+1
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__5
   EOR   g0
   STA   HARGS
   TXA
   EOR   g0+1
   STA   HARGS+1
   LDA   #$00
   EOR   __local_main__1
   STA   HARGS+2
   LDA   #$00
   EOR   __local_main__0
   STA   HARGS+3
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__5
   EOR   g0
   STA   __local_main__0
   LDA   __local_main__4
   EOR   g0+1
   STA   __local_main__1
   LDA   __local_main__0
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__1
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$4D
   STA   g0
   RTS
