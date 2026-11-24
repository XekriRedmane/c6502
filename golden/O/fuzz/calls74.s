SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
__zpabi_f0__p1_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f0__p2	EQU	$82
__zpabi_f0__p3_0	EQU	$83
__zpabi_f0__p3_1	EQU	$84
__local_main__0	EQU	$85
__local_main__1	EQU	$86
__local_main__2	EQU	$87
__local_main__3	EQU	$88
__local_main__5	EQU	$8A

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p2,__zpabi_f0__p3_0,__zpabi_f0__p3_1 locals=__local_f0__0 indirect=false in_cycle=false param_regs=A,X,,,,, carry=true
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f1 -> f2
; call f2 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$4F weight=6
; zp g1 size=1 at=auto def=true init=$37 weight=5
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

f2:
   SUBROUTINE

   ; prologue: 1 arg bytes, 3 local bytes, 3 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$04
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

   LDY   #$06
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $81
   LDA   g1
   SEC
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $80
   LDA   #$00
   BNE   .thread@0
   LDA   $81
   CMP   $80
   BEQ   .cmp_true@1
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   STA   $C2
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   $C1
   STA   $C0
   LDY   #$06
   LDA   (FP),Y
   ASL
   LDA   #$00
   ROL
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
   STA   $83
   STX   $82
   TXA
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   $81
   STA   $80
   LDA   $C2
   CLC
   ADC   $83
   STA   $83
   LDA   #$00
   ADC   $82
   STA   $82
   LDA   $C1
   ADC   $81
   STA   $81
   LDA   $C0
   ADC   $80
   STA   $80
   LDA   #$03
   CLC
   SBC   $83
   LDA   #$00
   SBC   $82
   LDA   #$00
   SBC   $81
   LDA   #$00
   SBC   $80
   LDA   #$00
   ROL
   STA   $81
   ASL
   CLC
   ADC   $81

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
   CLC
   LDA   FP
   ADC   #$06
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

   LDA   g0
   AND   #$07
   TAX
   LDA   g1
   CPX   #$00
   BEQ   .shift_done@9
.shift_loop@8:
   LSR
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   STA   $80
   EOR   #$FF
   STA   $C5
   LDA   #$00
   EOR   #$FF
   STA   $C4
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $C3
   STA   $C2
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
   BVC   .jcmp_novf@12
   EOR   #$80
.jcmp_novf@12:
   BPL   .loop@0_break
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   JSR   f2
   STA   $81
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $80
   LDA   $81
   CLC
   ADC   $C1
   STA   $83
   LDA   $80
   ADC   $C0
   STA   $82
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $81
   STA   $80
   LDA   $C5
   CLC
   ADC   $83
   STA   $C5
   LDA   $C4
   ADC   $82
   STA   $C4
   LDA   $C3
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
   LDA   g0
   CLC
   ADC   $C5
   STA   g0
   LDA   g1
   STA   $82
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   $81
   STA   $80
   LDA   $82
   SEC
   SBC   $C5
   LDA   #$00
   SBC   $C4
   LDA   $81
   SBC   $C3
   LDA   $80
   SBC   $C2
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $82
   LDA   g0
   STA   $81
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   $80
   LDA   $82
   EOR   $81
   STA   $81
   LDA   #$00
   EOR   $80
   ORA   $81
   BEQ   .cmp_end@22
   LDA   #$01
.cmp_end@22:

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

f0:
   SUBROUTINE

   LDA   #$02
   ORA   g1
   CMP   #$02
   BNE   .cmp_true@23
   CLC
   RTS
.cmp_true@23:
   SEC
   RTS

main:
   SUBROUTINE

   LDA   #$69
   STA   __zpabi_f0__p1_0
   LDA   #$7A
   STA   __zpabi_f0__p2
   LDA   #$E3
   STA   __zpabi_f0__p3_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p3_1
   LDX   #$01
   LDA   #$14
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__5
   LDA   g0
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_main__2
   STA   __local_main__1
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__2
   STA   __local_main__0
   LDA   __local_main__5
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

__zp_init:
   SUBROUTINE

   LDA   #$4F
   STA   g0
   LDA   #$37
   STA   g1
   RTS
