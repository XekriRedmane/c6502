SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$2A
__local_f3__0	EQU	$80
__local_f3__1	EQU	$81
__local_f3__2	EQU	$82
__local_f3__3	EQU	$83
__local_f3__4	EQU	$84
__local_f3__5	EQU	$85
__local_f3__pp	EQU	$86
__local_f3__6	EQU	$87

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false carry=true
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params=__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__pp,__local_f3__6 indirect=false in_cycle=false param_regs=A,X
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> rec
; call f1 -> f2
; call f1 -> f3
; call f1 -> rec
; call f2 -> f3
; call f2 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=4 at=auto def=true init=$2D,$00,$00,$00 weight=13
; zp g1 size=2 at=auto def=true init=$AE,$00 weight=6
; runtime hargs_size=32 hargs_need=28
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

   STA   __local_f3__pp
   STX   __local_f3__6
   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__0
   SEC
   LDA   #$00
   SBC   __local_f3__0
   STA   __local_f3__4
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f3__0
   LDA   __local_f3__4
   ASL
   STA   __local_f3__2
   LDA   __local_f3__0
   ROL
   STA   __local_f3__3
   LDA   __local_f3__2
   CLC
   ADC   __local_f3__4
   STA   __local_f3__1
   LDA   __local_f3__3
   ADC   __local_f3__0
   ORA   __local_f3__1
   BEQ   .and_false@1
   LDA   __local_f3__4
   SEC
   SBC   #$01
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BPL   .and_false@1
   LDA   #$01
   STA   __local_f3__3
   LDA   #$00
   BEQ   .and_end@2
.and_false@1:
   LDA   #$00
   STA   __local_f3__3
.and_end@2:
   ORA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   __local_f3__1
   LDA   __local_f3__3
   STA   __local_f3__5
   LDA   __local_f3__1
   AND   #$03
   STA   __local_f3__0
   LDY   #$00
   LDA   (__local_f3__pp),Y
   STA   __local_f3__3
   INY
   LDA   (__local_f3__pp),Y
   STA   __local_f3__2
   LDA   __local_f3__5
   ORA   __local_f3__1
   ORA   __local_f3__0
   BEQ   .and_false@3
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@3
   LDA   #$01
   STA   __local_f3__1
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_f3__1
.and_end@4:
   LDA   __local_f3__3
   CLC
   ADC   __local_f3__1
   STA   __local_f3__1
   LDA   __local_f3__2
   ADC   #$00
   LDY   #$01
   STA   (__local_f3__pp),Y
   LDA   __local_f3__1
   DEY
   STA   (__local_f3__pp),Y
   INY
   LDA   (__local_f3__pp),Y
   TAX
   DEY
   LDA   (__local_f3__pp),Y
   RTS

f2:
   SUBROUTINE

   ; prologue: 9 arg bytes, 6 local bytes, 4 callee-saved bytes
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

   LDY   #$11
   LDA   (FP),Y
   CLC
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $80
   LDA   #$00
   CMP   #$00
   BNE   .cmp_true@7
   LDA   (FP),Y
   CMP   $80
   BNE   .cmp_true@7
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   STA   $C2
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   $C1
   STA   $C0
   LDY   #$11
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$05
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   LDX   $81
   LDA   $80
   JSR   f3
   STA   $83
   STX   $82
   TXA
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   $81
   STA   $80
   LDA   $C2
   EOR   $83
   STA   $87
   LDA   #$00
   EOR   $82
   STA   $86
   LDA   $C1
   EOR   $81
   STA   $85
   LDA   $C0
   EOR   $80
   STA   $84
   LDY   #$05
   LDA   (FP),Y
   STA   $83
   INY
   LDA   (FP),Y
   STA   $82
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   $81
   STA   $80
   LDA   $87
   CLC
   ADC   $83
   STA   $C3
   LDA   $86
   ADC   $82
   STA   $C2
   LDA   $85
   ADC   $81
   STA   $C1
   LDA   $84
   ADC   $80
   STA   $C0
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   LDA   g0+2
   STA   $81
   LDA   g0+3
   STA   $80
   LDA   $83
   CLC
   ADC   $C3
   STA   g0
   LDA   $82
   ADC   $C2
   STA   g0+1
   LDA   $81
   ADC   $C1
   STA   g0+2
   LDA   $80
   ADC   $C0
   STA   g0+3
   LDA   g0
   STA   $81
   LDA   g0+1
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
   LDY   #$10
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BEQ   .cmp_true@16
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   $86
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   $85
   STA   $84
   SEC
   LDA   #$00
   SBC   $C3
   STA   $83
   LDA   #$00
   SBC   $C2
   STA   $82
   LDA   #$00
   SBC   $C1
   STA   $81
   LDA   #$00
   SBC   $C0
   STA   $80
   LDA   $86
   SEC
   SBC   $83
   STA   $83
   LDA   #$00
   SBC   $82
   STA   $82
   LDA   $85
   SBC   $81
   STA   $81
   LDA   $84
   SBC   $80
   ORA   $81
   ORA   $82
   ORA   $83
   BEQ   .cmp_end@21
   LDA   #$01
.cmp_end@21:

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
   CLC
   LDA   FP
   ADC   #$11
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
   RTS

f1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes, 8 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
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

   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   LDA   g0+2
   STA   $81
   LDA   g0+3
   STA   $80
   LDA   $83
   STA   $C7
   LDA   $82
   STA   $C6
   LDA   $81
   STA   $C5
   LDA   $80
   AND   #$03
   STA   $C4
   LDA   $83
   CLC
   ADC   #$FF
   STA   $83
   LDA   $82
   ADC   #$FF
   STA   $82
   LDA   $81
   ADC   #$FF
   STA   $81
   LDA   $80
   ADC   #$FF
   STA   $80
   SEC
   LDA   SSP
   SBC   #$09
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
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$07
   INY
   STA   (SSP),Y
   JSR   f2
   ORA   #$00
   BEQ   .if_else@8
   LDA   #$02
   STA   $C3
   LDA   #$00
   STA   $C2
   STA   $C1
   STA   $C0
   JMP   .if_end@7
.if_else@8:
   LDA   #$FA
   STA   $C3
   LDA   #$FF
   STA   $C2
   STA   $C1
   STA   $C0
.if_end@7:
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   LDA   g0+2
   STA   $81
   LDA   g0+3
   STA   $80
   LDA   $83
   CLC
   ADC   $C3
   STA   g0
   LDA   $82
   ADC   $C2
   STA   g0+1
   LDA   $81
   ADC   $C1
   STA   g0+2
   LDA   $80
   ADC   $C0
   STA   g0+3
   LDA   $C3
   LDY   #$09
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$09
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   LDX   $81
   LDA   $80
   JSR   f3
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   $81
   STA   $80
   LDA   g0
   SEC
   SBC   $83
   LDA   g0+1
   SBC   $82
   LDA   g0+2
   SBC   $81
   LDA   g0+3
   SBC   $80
   BVC   .cmp_novf@24
   EOR   #$80
.cmp_novf@24:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   LDA   $C7
   SEC
   SBC   g0
   STA   $83
   LDA   $C6
   SBC   g0+1
   STA   $86
   LDA   $C5
   SBC   g0+2
   STA   $85
   LDA   $C4
   SBC   g0+3
   STA   $84
   LDA   $C0
   BNE   .thread@1
   LDA   $C1
   BNE   .thread@1
   LDA   $C2
   BNE   .thread@1
   LDA   $C3
   CMP   #$07
   BEQ   .cmp_true@25
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   STA   $82
   LDA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   STA   $81
   STA   $80
   LDA   $83
   ORA   $82
   STA   $83
   LDA   $86
   STA   $82
   LDA   $85
   ORA   $81
   STA   $81
   LDA   $84
   ORA   $80
   ORA   $81
   ORA   $82
   ORA   $83
   BEQ   .cmp_end@30
   LDA   #$01
.cmp_end@30:

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
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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

   ; prologue: 11 arg bytes, 1 local bytes, 1 callee-saved bytes
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

   LDY   #$04
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   LDY   #$0D
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   LDY   #$00
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod64
   LDA   HARGS+24
   STA   $83
   LDA   HARGS+25
   STA   $82
   LDA   HARGS+26
   STA   $81
   LDA   HARGS+27
   AND   #$03
   STA   $80
   LDA   $83
   CLC
   SBC   #$FF
   LDA   $82
   SBC   #$00
   LDA   $81
   SBC   #$00
   LDA   $80
   SBC   #$00
   LDA   #$00
   ROL
   STA   $81
   LDA   #$FF
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $80
   LDA   #$00
   CMP   #$00
   BNE   .thread@2
   LDA   $81
   CMP   $80
   BEQ   .cmp_true@31
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   STA   $C0
   LDA   #$02
   LDY   #$0C
   ORA   (FP),Y
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
   LDA   $C0
   BEQ   .cmp_end@34
   LDA   #$01
.cmp_end@34:
   CMP   #$01

   ; epilogue
   PHP
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$0E
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
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$2B
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$57
   INY
   STA   (SSP),Y
   LDA   #$13
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   STA   $84
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   $81
   STA   $80
   LDA   g0
   EOR   $83
   STA   $83
   LDA   g0+1
   EOR   $82
   STA   $82
   LDA   g0+2
   EOR   $81
   STA   $81
   LDA   g0+3
   EOR   $80
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   STA   g1+1
   LDA   #$2D
   STA   g0
   LDA   #$AE
   STA   g1
   RTS
