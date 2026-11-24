SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f4__0	EQU	$80
__zpabi_f0__p1_0	EQU	$80
__local_f4__1	EQU	$81
__zpabi_f0__p1_1	EQU	$81
__local_f4__2	EQU	$82
__zpabi_f0__p1_2	EQU	$82
__local_f4__3	EQU	$83
__zpabi_f0__p1_3	EQU	$83
__zpabi_f0__p2	EQU	$84
__local_f1__0	EQU	$85
__zpabi_f2__p1_0	EQU	$85
__zpabi_f2__p1_1	EQU	$86
__zpabi_f2__p1_2	EQU	$87
__zpabi_f2__p1_3	EQU	$88
__local_main__0	EQU	$89
__local_main__2	EQU	$8B
__local_main__3	EQU	$8C
__local_f0__p0_1	EQU	$91
__local_f0__p0_0	EQU	$92
__local_f0__0	EQU	$93
__local_f2__p0_1	EQU	$96
__local_f2__p0_0	EQU	$97

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2 locals=__local_f0__p0_1,__local_f0__p0_0,__local_f0__0,__local_f0__1,__local_f0__2 indirect=false in_cycle=false param_regs=A,X,,,,,
; def f1 params= locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3 locals=__local_f2__p0_1,__local_f2__p0_0,__local_f2__0 indirect=false in_cycle=false param_regs=A,X,,,,
; def f3 params= locals= indirect=true in_cycle=false
; def f4 params= locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call main -> f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$8D,$00 weight=15
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f4:
   SUBROUTINE

   ; prologue: 1 arg bytes, 0 local bytes
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

   INY
   LDA   (FP),Y
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f4__2
   LDA   g0
   AND   #$07
   TAX
   LDY   #$03
   LDA   (FP),Y
   STA   __local_f4__0
   LDA   __local_f4__2
   STA   __local_f4__1
   CPX   #$00
   BEQ   .shift_done@3
.shift_loop@2:
   LDA   __local_f4__1
   ASL
   ROR   __local_f4__1
   ROR   __local_f4__0
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDY   #$03
   LDA   (FP),Y
   EOR   __local_f4__0
   STA   __local_f4__3
   LDA   __local_f4__2
   EOR   __local_f4__1
   STA   __local_f4__2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f4__1
   STA   __local_f4__0
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@0
   LDA   __local_f4__3
   ORA   __local_f4__2
   ORA   __local_f4__1
   ORA   __local_f4__0
   BNE   .and_end@1
.and_false@0:
.and_end@1:
   LDY   #$03
   LDA   (FP),Y
   BNE   .or_true@2
   LDA   g0
   ORA   g0+1
   BEQ   .or_end@3
.or_true@2:
.or_end@3:
   LDY   #$03
   LDA   (FP),Y
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   TAX
   LDY   #$03
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$03
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

   ; prologue: 8 arg bytes, 7 local bytes, 7 callee-saved bytes
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

   LDY   #$0E
   LDA   (FP),Y
   CLC
   ADC   #$64
   STA   $C3
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $C2
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $C1
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $C0
   LDY   #$0C
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+1
   LDA   (FP),Y
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+3
   STA   $80
   ORA   #$01
   STA   HARGS+4
   LDA   $C3
   STA   HARGS
   LDA   $C2
   STA   HARGS+1
   LDA   $C1
   STA   HARGS+2
   LDA   $C0
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   SEC
   LDA   #$00
   SBC   $83
   STA   $81
   LDA   #$00
   CMP   g0+1
   BNE   .cmp_true@8
   LDY   #$0C
   LDA   (FP),Y
   CMP   g0
   BNE   .cmp_true@8
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   $80
   ORA   $81
   STA   $C6
   SEC
   SBC   $83
   STA   $C3
   LDA   #$00
   SBC   $82
   STA   $C2
   LDA   #<f4
   STA   $C4
   LDA   #>f4
   STA   $C5
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
   BVC   .jcmp_novf@11
   EOR   #$80
.jcmp_novf@11:
   BPL   .loop@0_break
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDA   $C4
   STA   DPTR
   LDA   $C5
   STA   DPTR+1
   JSR   icall
   CLC
   ADC   $C1
   STA   $81
   TXA
   ADC   $C0
   STA   $80
   LDA   $C3
   CLC
   ADC   $81
   STA   $C3
   LDA   $C2
   ADC   $80
   STA   $C2
   INC   $C1
   BNE   .loop@0_start
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$01
   CLC
   SBC   $C6
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
   PLA
   TAX
   PLA
   RTS

f2:
   SUBROUTINE

   CPX   #$7F
   BNE   .cmp_true@12
   CMP   #$FF
   BEQ   .cmp_end@13
.cmp_true@12:
.cmp_end@13:
   LDA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   __local_f2__p0_0
   STA   __local_f2__p0_1
   SEC
   LDA   #$00
   SBC   #$80
   LDA   __local_f2__p0_0
   SBC   #$00
   LDA   __local_f2__p0_1
   SBC   #$00
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   ASL
   LDA   #$00
   ROL
   EOR   #$FF
   BNE   .or_true@6
   LDA   g0
   ORA   g0+1
   BNE   .or_true@6
   LDA   #$00
   RTS
.or_true@6:
   LDA   #$01
   RTS

f1:
   SUBROUTINE

   LDA   g0+1
   CMP   g0+1
   BNE   .thread@0
   LDA   g0
   CMP   g0
   BEQ   .cmp_true@18
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   STA   __local_f1__0
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__0
   STA   HARGS+8
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   SEC
   LDA   #$00
   SBC   __local_f0__p0_0
   STA   __local_f0__0
   JSR   f1
   LDA   __zpabi_f0__p1_0
   EOR   __zpabi_f0__p1_0
   STA   __local_f0__p0_0
   LDA   __zpabi_f0__p1_1
   EOR   __zpabi_f0__p1_1
   STA   __local_f0__p0_1
   LDA   __zpabi_f0__p2
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f0__p2
   STA   HARGS
   LDA   __zpabi_f0__p1_0
   STA   HARGS+4
   LDA   __zpabi_f0__p1_1
   STA   HARGS+5
   LDA   __zpabi_f0__p1_2
   STA   HARGS+6
   LDA   __zpabi_f0__p1_3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   __zpabi_f2__p1_0
   LDA   HARGS+9
   STA   __zpabi_f2__p1_1
   LDA   HARGS+10
   STA   __zpabi_f2__p1_2
   LDA   HARGS+11
   STA   __zpabi_f2__p1_3
   LDX   __local_f0__p0_1
   LDA   __local_f0__p0_0
   JSR   f2
   LDA   __local_f0__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$B1
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDA   #$2E
   STA   __zpabi_f0__p2
   LDX   #$00
   LDA   #$70
   JSR   f0
   LDA   HARGS+8
   EOR   g0
   STA   __local_main__3
   LDA   HARGS+9
   EOR   g0+1
   STA   __local_main__2
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$8D
   STA   g0
   RTS
