SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f2__0	EQU	$80
__zpabi_f1__p1_0	EQU	$80
__local_f2__1	EQU	$81
__zpabi_f1__p1_1	EQU	$81
__local_f2__2	EQU	$82
__zpabi_f1__p1_2	EQU	$82
__local_f2__3	EQU	$83
__zpabi_f1__p1_3	EQU	$83
__local_f1__0	EQU	$84
__local_f2__4	EQU	$84
__local_f1__1	EQU	$85
__local_f1__2	EQU	$86
__local_f1__3	EQU	$87
__local_f1__4	EQU	$88
__local_f1__5	EQU	$89

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params=__zpabi_f1__p0,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6 indirect=false in_cycle=false param_regs=A,,,, carry=true
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f1
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$4D weight=14
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

f2:
   SUBROUTINE

   ; prologue: 5 arg bytes, 0 local bytes
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

   LDY   #$07
   LDA   (FP),Y
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__4
   LDY   #$03
   LDA   (FP),Y
   AND   (FP),Y
   STA   __local_f2__3
   INY
   LDA   (FP),Y
   AND   (FP),Y
   STA   __local_f2__2
   INY
   LDA   (FP),Y
   AND   (FP),Y
   STA   __local_f2__1
   INY
   LDA   (FP),Y
   AND   (FP),Y
   STA   __local_f2__0
   LDA   __local_f2__4
   CMP   __local_f2__0
   BNE   .thread@0
   CMP   __local_f2__1
   BNE   .thread@0
   CMP   __local_f2__2
   BNE   .thread@0
   INY
   LDA   (FP),Y
   CMP   __local_f2__3
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDX   #$00

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$07
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

   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   __local_f1__5
   STA   __local_f1__4
   LDA   #$03
   EOR   __zpabi_f1__p1_0
   STA   __local_f1__3
   LDA   #$00
   EOR   __zpabi_f1__p1_1
   STA   __local_f1__2
   LDA   #$00
   EOR   __zpabi_f1__p1_2
   STA   __local_f1__1
   LDA   #$00
   EOR   __zpabi_f1__p1_3
   STA   __local_f1__0
   LDA   __local_f1__3
   SEC
   SBC   g0
   LDA   __local_f1__2
   SBC   #$00
   LDA   __local_f1__1
   SBC   __local_f1__5
   LDA   __local_f1__0
   SBC   __local_f1__4
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   ; prologue: 2 arg bytes, 7 local bytes, 7 callee-saved bytes
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

   LDA   g0
   EOR   g0
   STA   $85
   LDA   g0
   CLC
   SBC   g0
   LDA   #$00
   ROL
   STA   $84
   LDA   $85
   SEC
   SBC   $84
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $C6
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   $86
   STA   $85
   LDA   g0
   BEQ   .lnot_true@11
   LDA   #$00
   BEQ   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   STA   $84
   LDA   $C6
   ORA   $86
   ORA   $85
   BEQ   .and_false@0
   LDA   $C6
   ORA   $86
   ORA   $85
   BNE   .and_end@1
.and_false@0:
.and_end@1:
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $87
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $86
   LDA   $84
   EOR   $C6
   CLC
   ADC   $87
   STA   $85
   LDA   $86
   ADC   #$00
   STA   $84
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $84
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f2
   STA   $C4
   LDA   #>f2
   STA   $C5
   LDA   g0
   STA   $C3
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $C2
   STA   $C1
   STA   $C0
   LDA   g0
   ORA   #$01
   STA   HARGS+1
   LDA   g0
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+2
   STA   $84
   SEC
   LDA   SSP
   SBC   #$05
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
   LDA   $C1
   INY
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $C4
   STA   DPTR
   LDA   $C5
   STA   DPTR+1
   JSR   icall
   LDA   #$00
   STA   __zpabi_f1__p1_0
   STA   __zpabi_f1__p1_1
   STA   __zpabi_f1__p1_2
   STA   __zpabi_f1__p1_3
   LDA   $C6
   JSR   f1
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $85
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   ORA   $85
   BNE   .or_true@4
   LDA   g0
   BNE   .or_true@4
   LDA   #$00
   STA   $88
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   STA   $88
.or_end@5:
   LDA   g0
   STA   $86
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $85
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $84
   LDA   $86
   EOR   $85
   STA   $85
   LDA   #$00
   EOR   $84
   STA   $84
   LDA   $85
   SEC
   SBC   $88
   LDA   $84
   SBC   #$00
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   EOR   #$01

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
   ADC   #$0B
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes, 2 callee-saved bytes
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

   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$03
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f0
   STA   $84
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   $83
   STA   $82
   STA   $81
   LDA   g0
   STA   $80
   LDA   $84
   EOR   $80
   STA   $C1
   STA   HARGS
   LDA   $83
   STA   $C0
   STA   HARGS+2
   STA   HARGS+3
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
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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

   LDA   #$4D
   STA   g0
   RTS
