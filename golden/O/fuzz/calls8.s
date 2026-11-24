SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__local_f2__0	EQU	$80
__local_f3__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__local_f2__1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__local_f2__2	EQU	$82
__zpabi_f1__p1_0	EQU	$82
__zpabi_f1__p1_1	EQU	$83
__local_f1__0	EQU	$84
__local_f1__1	EQU	$85
__local_rec__0	EQU	$86
__local_rec__1	EQU	$87
__local_rec__2	EQU	$88
__local_rec__3	EQU	$89
__local_rec__4	EQU	$8A

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2_0,__zpabi_f1__p2_1 locals=__local_f1__0,__local_f1__1 indirect=false in_cycle=false param_regs=,,,,A,X
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false
; def f3 params= locals=__local_f3__0 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f1
; call f1 -> rec
; call f3 -> rec
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$B4 weight=35
; zp g0 size=2 at=auto def=true init=$20,$00 weight=7
; runtime hargs_size=32 hargs_need=9
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

f3:
   SUBROUTINE

   ; prologue: 6 arg bytes, 0 local bytes
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

   LDA   g1
   STA   __local_f3__0
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   DEY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   __local_f3__0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$03
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$08
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

   LDA   g1
   BEQ   .and_false@1
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@1
   LDA   #$01
   STA   __local_f2__0
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   STA   __local_f2__0
.and_end@2:
   LDA   g0
   AND   #$07
   TAX
   LDA   g0
   STA   __local_f2__1
   LDA   g0+1
   STA   __local_f2__2
   CPX   #$00
   BEQ   .shift_done@2
.shift_loop@1:
   ASL   __local_f2__1
   ROL   __local_f2__2
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   LDA   __local_f2__0
   SEC
   SBC   __local_f2__1
   EOR   #$FF
   LDX   #$00
   RTS

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p1_1
   CMP   #$7F
   BNE   .cmp_true@3
   LDA   __zpabi_f1__p1_0
   CMP   #$FF
   BNE   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   ORA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDA   __zpabi_f1__p0_0
   STA   HARGS
   LDA   __zpabi_f1__p0_1
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+6
   ORA   __zpabi_f1__p0_0
   STA   __local_f1__1
   LDA   HARGS+7
   ORA   __zpabi_f1__p0_1
   STA   __local_f1__0
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
   LDA   __local_f1__1
   INY
   STA   (SSP),Y
   LDA   __local_f1__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDX   g0+1
   LDA   g0
   RTS

f0:
   SUBROUTINE

   ; prologue: 2 arg bytes, 8 local bytes, 8 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
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

   LDA   g1
   ORA   g1
   STA   $C5
   LDY   #$0B
   LDA   (FP),Y
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   LDA   g1
   CPX   #$00
   BEQ   .shift_done@7
.shift_loop@6:
   LSR
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   STA   $84
   AND   #$07
   TAX
   BEQ   .shift_done@9
.shift_loop@8:
   LSR   $C5
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   #<f3
   STA   $C6
   LDA   #>f3
   STA   $C7
   LDA   #$00
   STA   $C4
   STA   $C3
   STA   $C2
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@0_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@10
   EOR   #$80
.jcmp_novf@10:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   g1
   STA   $88
   LDA   #$00
   SEC
   SBC   $88
   STA   $87
   LDA   $88
   SBC   #$00
   STA   $88
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   $86
   STA   $85
   LDA   g1
   AND   #$07
   TAX
   LDA   g1
   STA   $84
   CPX   #$00
   BEQ   .shift_done@14
.shift_loop@13:
   LSR   $84
   DEX
   BNE   .shift_loop@13
.shift_done@14:
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $87
   LDY   #$01
   STA   (SSP),Y
   LDA   $88
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
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $C6
   STA   DPTR
   LDA   $C7
   STA   DPTR+1
   JSR   icall
   STA   $85
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $84
   LDA   $85
   CLC
   ADC   $C1
   STA   $87
   LDA   $84
   ADC   $C0
   STA   $86
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   $85
   STA   $84
   LDA   $C5
   CLC
   ADC   $87
   STA   $C5
   LDA   $C4
   ADC   $86
   STA   $C4
   LDA   $C3
   ADC   $85
   STA   $C3
   LDA   $C2
   ADC   $84
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   CLC
   ADC   $C5
   STA   g1
   LDA   #$00
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@1_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@19
   EOR   #$80
.jcmp_novf@19:
   BPL   .loop@1_break
   LDA   $C5
   STA   __zpabi_f1__p0_0
   LDA   $C4
   STA   __zpabi_f1__p0_1
   LDA   #$01
   STA   __zpabi_f1__p1_0
   LDA   #$00
   STA   __zpabi_f1__p1_1
   LDX   #$7F
   LDA   #$FF
   JSR   f1
   CLC
   ADC   $C1
   STA   $85
   TXA
   ADC   $C0
   STA   $84
   LDA   $C5
   CLC
   ADC   $85
   STA   $C5
   LDA   $C4
   ADC   $84
   STA   $C4
   BCC   .carry_inc@1
   INC   $C3
   BNE   .carry_inc@1
   INC   $C2
.carry_inc@1:
   INC   $C1
   BNE   .loop@1_start
   INC   $C0
   JMP   .loop@1_start
.loop@1_break:
   LDA   g1
   CLC
   ADC   $C5
   STA   g1
   SEC

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
   PLP
   RTS

main:
   SUBROUTINE

   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$B9
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   STA   $84
   LDA   g0
   STA   $82
   LDA   g0+1
   STA   $81
   LDA   $82
   EOR   g1
   STA   $83
   LDA   $81
   STA   $82
   LDA   $84
   EOR   $83
   STA   $83
   LDA   #$00
   EOR   $82
   STA   $82
   LDA   $83
   LDX   $82
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$20
   STA   g0
   LDA   #$B4
   STA   g1
   RTS
