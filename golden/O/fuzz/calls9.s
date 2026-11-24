SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__local_f3__0	EQU	$80
__local_main__0	EQU	$80
__local_f3__1	EQU	$81
__local_main__1	EQU	$81
__local_f3__it_1	EQU	$82
__local_main__2	EQU	$82
__local_f3__it_0	EQU	$83
__local_main__3	EQU	$83
__local_f3__l0_3	EQU	$84
__local_main__4	EQU	$84
__local_f3__l0_2	EQU	$85
__local_main__5	EQU	$85
__local_f3__l0_1	EQU	$86
__local_f3__l0_0	EQU	$87
__local_f3__2	EQU	$88
__local_main__8	EQU	$88
__local_f3__3	EQU	$89
__local_f3__pp	EQU	$8A
__local_f0__0	EQU	$8B
__local_f3__4	EQU	$8B
__local_f0__1	EQU	$8C
__local_f0__2	EQU	$8D

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3 indirect=false in_cycle=false carry=true
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params=__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1,__local_f3__it_1,__local_f3__it_0,__local_f3__l0_3,__local_f3__l0_2,__local_f3__l0_1,__local_f3__l0_0,__local_f3__2,__local_f3__3,__local_f3__pp,__local_f3__4,__local_f3__5,__local_f3__6 indirect=false in_cycle=false param_regs=A,X
; def f4 params= locals= indirect=false in_cycle=false carry=true
; def f5 params= locals=__local_f5__0,__local_f5__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f1 -> f5
; call f1 -> rec
; call f2 -> rec
; call f3 -> f5
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$2F weight=9
; zp g0 size=2 at=auto def=true init=$44,$00 weight=10
; runtime hargs_size=32 hargs_need=12
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

f5:
   SUBROUTINE

   LDA   g0
   ORA   g0+1
   BEQ   .and_false@1
   LDA   #$01
   BNE   .and_end@2
.and_false@1:
   LDA   #$00
.and_end@2:
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDX   #$00
   RTS

f4:
   SUBROUTINE

   CLC
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__pp
   STX   __local_f3__4
   LDA   g1
   BNE   .or_true@3
   LDA   g0+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g0
   SEC
   SBC   g2
   LDA   g0+1
   SBC   g2+1
   LDA   __local_f3__1
   SBC   g2+2
   LDA   __local_f3__0
   SBC   g2+3
   BCS   .or_true@3
   LDA   #$00
   STA   __local_f3__3
   STA   __local_f3__l0_1
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   __local_f3__3
   LDA   #$00
   STA   __local_f3__l0_1
.or_end@4:
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   __local_f3__l0_2
   STA   __local_f3__l0_3
   LDA   g2
   LDX   g2+1
   STX   __local_f3__it_1
   LDX   g2+2
   STX   __local_f3__1
   LDX   g2+3
   STX   __local_f3__0
   SEC
   SBC   __local_f3__3
   LDA   __local_f3__it_1
   SBC   #$00
   LDA   __local_f3__1
   SBC   __local_f3__l0_2
   LDA   __local_f3__0
   SBC   __local_f3__l0_3
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__2
   EOR   #$01
   CLC
   LDY   #$00
   ADC   (__local_f3__pp),Y
   STA   __local_f3__1
   INY
   LDA   (__local_f3__pp),Y
   ADC   #$00
   STA   (__local_f3__pp),Y
   LDA   __local_f3__1
   DEY
   STA   (__local_f3__pp),Y
   LDA   __local_f3__3
   STA   __local_f3__l0_0
   LDA   #$00
   STA   __local_f3__it_0
   STY   __local_f3__it_1
.loop@0_start:
   LDA   __local_f3__it_0
   SEC
   SBC   #$03
   LDA   __local_f3__it_1
   SBC   #$00
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BPL   .loop@0_break
   JSR   f5
   CLC
   ADC   __local_f3__it_0
   STA   __local_f3__1
   TXA
   ADC   __local_f3__it_1
   STA   __local_f3__0
   LDA   __local_f3__l0_0
   CLC
   ADC   __local_f3__1
   STA   __local_f3__l0_0
   LDA   __local_f3__l0_1
   ADC   __local_f3__0
   STA   __local_f3__l0_1
   BCC   .carry_inc@0
   INC   __local_f3__l0_2
   BNE   .carry_inc@0
   INC   __local_f3__l0_3
.carry_inc@0:
   INC   __local_f3__it_0
   BNE   .loop@0_start
   INC   __local_f3__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g2
   CLC
   ADC   __local_f3__l0_0
   STA   g2
   LDA   g2+1
   ADC   __local_f3__l0_1
   STA   g2+1
   LDA   g2+2
   ADC   __local_f3__l0_2
   STA   g2+2
   LDA   g2+3
   ADC   __local_f3__l0_3
   STA   g2+3
   LDA   __local_f3__2
   CLC
   SBC   __local_f3__3
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   HARGS
   LDY   #$00
   LDA   (__local_f3__pp),Y
   ORA   #$01
   STA   HARGS+2
   INY
   LDA   (__local_f3__pp),Y
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   RTS

f2:
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

   LDA   g0
   ORA   g0+1
   BNE   .or_true@5
   LDA   g1
   BNE   .or_true@5
   LDA   #$FF
   STA   $85
   STA   $84
   JMP   .or_end@6
.or_true@5:
   LDA   #$FE
   STA   $85
   LDA   #$FF
   STA   $84
.or_end@6:
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
.sx_neg@8:
   STA   $81
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDA   #$FF
   ORA   $81
   ORA   $80
   BNE   .or_true@7
   LDY   #$07
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDA   #$00
   STA   $81
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   STA   $81
.or_end@8:
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   #$00
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   STA   $80
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   $80

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0B
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

f1:
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

   LDA   g1
   STA   $82
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@10:
   BCC   .mul_skip@11
   CLC
.mul_skip@11:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@10
   STA   $81
   LDA   HARGS
   STA   $80
   LDA   $82
   SEC
   SBC   $80
   STA   $C1
   LDA   #$00
   SBC   $81
   STA   $C0
   JSR   f5
   STA   $81
   STX   $80
   LDA   $C1
   EOR   $81
   STA   $C1
   LDA   $C0
   EOR   $80
   STA   $C0
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   $81
   CLC
   ADC   $C1
   STA   g0
   LDA   $80
   ADC   $C0
   STA   g0+1
   LDA   g2
   STA   $81
   LDA   g2+1
   STA   $80
   LDA   $81
   CLC
   ADC   $C1
   STA   $81
   LDA   $80
   ADC   $C0
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
   CLC
   ADC   $C1
   STA   HARGS+8
   TXA
   ADC   $C0
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11

   ; epilogue
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
   RTS

f0:
   SUBROUTINE

   LDA   g0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f0__2
   LDA   g1
   ORA   g0
   LDX   __local_f0__2
   STX   __local_f0__0
   CLC
   ADC   g0
   STA   __local_f0__1
   LDA   __local_f0__0
   ADC   __local_f0__2
   ORA   __local_f0__1
   BEQ   .cmp_end@14
   SEC
   RTS
.cmp_end@14:
   CMP   #$01
   RTS

main:
   SUBROUTINE

   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g0+1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_main__2
   LDA   g0
   EOR   g1
   LDX   __local_main__2
   STX   __local_main__5
   STX   __local_main__4
   EOR   g2
   STA   __local_main__3
   LDA   g0+1
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g2+2
   STA   __local_main__1
   LDA   __local_main__4
   EOR   g2+3
   STA   __local_main__0
   LDA   __local_main__8
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

g2:
   DC.L  $00000001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$44
   STA   g0
   LDA   #$2F
   STA   g1
   RTS
