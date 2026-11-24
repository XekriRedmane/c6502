SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g2	EQU	$27
__zpabi_f1__p1_0	EQU	$80
__local_f3__1	EQU	$81
__zpabi_f1__p1_1	EQU	$81
__local_f3__2	EQU	$82
__zpabi_f1__p1_2	EQU	$82
__local_f3__3	EQU	$83
__zpabi_f1__p1_3	EQU	$83
__local_f3__4	EQU	$84

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3 locals= indirect=false in_cycle=false param_regs=A,X,,,,
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f1
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$3B weight=25
; zp g2 size=4 at=auto def=true init=$41,$00,$00,$00 weight=19
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

f3:
   SUBROUTINE

   ; prologue: 7 arg bytes, 0 local bytes
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

   LDA   #$01
   CLC
   ADC   g0
   STA   __local_f3__4
   LDA   #$00
   ADC   g0+1
   STA   __local_f3__3
   LDA   #$00
   ADC   g0+2
   STA   __local_f3__2
   LDA   #$00
   ADC   g0+3
   STA   __local_f3__1
   LDA   #$64
   AND   g1
   SEC
   SBC   __local_f3__4
   LDA   #$00
   SBC   __local_f3__3
   LDA   #$00
   SBC   __local_f3__2
   LDA   #$00
   SBC   __local_f3__1
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__1
   LDA   g1
   BNE   .or_true@0
   LDA   __local_f3__1
   AND   #$07
   TAX
   LDA   g1
   CPX   #$00
   BEQ   .shift_done@2
.shift_loop@1:
   LSR
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   ORA   #$00
   BEQ   .or_end@1
.or_true@0:
.or_end@1:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$09
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

   ; prologue: 0 arg bytes, 6 local bytes, 6 callee-saved bytes
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
   LDA   $C4
   INY
   STA   (FP),Y
   LDA   $C5
   INY
   STA   (FP),Y

   LDA   g2
   SEC
   SBC   g2
   LDA   g2+1
   SBC   g2+1
   LDA   g2+2
   SBC   g2+2
   LDA   g2+3
   SBC   g2+3
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BPL   .or_true@6
   LDA   g1
   STA   $82
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   $81
   STA   $80
   LDA   $82
   SEC
   SBC   g2
   LDA   #$00
   SBC   g2+1
   LDA   $81
   SBC   g2+2
   LDA   $80
   SBC   g2+3
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BMI   .or_true@6
   LDA   #$00
   STA   $C5
   STA   $C4
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   STA   $C5
   LDA   #$00
   STA   $C4
.or_end@7:
   LDA   g1
   SEC
   SBC   g1
   LDA   #$00
   ROL
   STA   $82
   LDA   g2
   STA   $81
   LDA   g2+1
   STA   $80
   LDA   $82
   SEC
   SBC   $81
   STA   $C3
   LDA   #$00
   SBC   $80
   STA   $C2
   LDA   #<f3
   STA   $C0
   LDA   #>f3
   STA   $C1
   LDA   g1
   STA   $81
   STA   HARGS
   LDA   g2
   STA   $80
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   $82
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   $81
   AND   $C5
   STA   $81
   LDA   $80
   AND   $C4
   STA   $80
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C3
   LDY   #$01
   STA   (SSP),Y
   LDA   $C2
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
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
   LDA   $C0
   STA   DPTR
   LDA   $C1
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   #$00
   EOR   $80
   STA   $82
   LDA   g1
   BNE   .or_true@8
   LDA   $C3
   ORA   $C2
   BNE   .or_true@8
   LDA   #$00
   STA   $81
   JMP   .or_end@9
.or_true@8:
   LDA   #$01
   STA   $81
.or_end@9:
   LDA   $82
   SEC
   SBC   $81
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL

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
   CLC
   LDA   FP
   ADC   #$08
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

   LDX   #$00
   TXA
   RTS

f0:
   SUBROUTINE

   ; prologue: 14 arg bytes, 5 local bytes, 5 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
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

   LDA   g2
   STA   $88
   LDX   g2+1
   STA   $87
   STX   $86
   LDA   g2+2
   STA   $85
   LDA   g2+3
   STA   $84
   LDA   $87
   STA   __zpabi_f1__p1_0
   LDA   $86
   STA   __zpabi_f1__p1_1
   LDA   $85
   STA   __zpabi_f1__p1_2
   LDA   $84
   STA   __zpabi_f1__p1_3
   LDA   $88
   JSR   f1
   STA   $84
   LDY   #$10
   LDA   (FP),Y
   EOR   $84
   STA   $C2
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
   LDA   #<f3
   STA   $C3
   LDA   #>f3
   STA   $C4
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
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   g1
   STA   $88
   STA   $86
   LDA   g2
   STA   $87
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   $85
   STA   $84
   LDA   $86
   SEC
   LDY   #$0C
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   $85
   INY
   SBC   (FP),Y
   LDA   $84
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $84
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $88
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $C3
   STA   DPTR
   LDA   $C4
   STA   DPTR+1
   JSR   icall
   CLC
   ADC   $C1
   CLC
   ADC   $C2
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   g0
   CLC
   ADC   $C2
   STA   g0
   BCC   .carry_inc@1
   INC   g0+1
   BNE   .carry_inc@1
   INC   g0+2
   BNE   .carry_inc@1
   INC   g0+3
.carry_inc@1:
   LDA   $C2
   SEC
   LDY   #$0C
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
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
   CLC
   LDA   FP
   ADC   #$15
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
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   DEY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$3E
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$0D
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$5A
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   STA   $84
   LDA   g0
   EOR   g1
   STA   $83
   LDA   g0+1
   STA   $82
   LDA   g0+2
   STA   $81
   LDA   g0+3
   STA   $80
   LDA   $83
   EOR   g2
   STA   $83
   LDA   $82
   EOR   g2+1
   STA   $82
   LDA   $81
   EOR   g2+2
   STA   $81
   LDA   $80
   EOR   g2+3
   STA   $80
   LDA   $84
   EOR   $83
   STA   $C1
   LDA   #$00
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
   DC.L  $00000029

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   LDA   #$3B
   STA   g1
   LDA   #$41
   STA   g2
   RTS
