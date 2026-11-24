SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_f2__p0_0	EQU	$80
__zpabi_f2__p0_1	EQU	$81
__zpabi_f2__p0_2	EQU	$82
__zpabi_f2__p0_3	EQU	$83
__zpabi_f2__p0_4	EQU	$84
__zpabi_f2__p0_5	EQU	$85
__local_f2__0	EQU	$86
__local_f2__1	EQU	$87
__local_f2__2	EQU	$88
__local_f2__3	EQU	$89
__local_f2__4	EQU	$8A
__local_f2__5	EQU	$8B

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p0_4,__zpabi_f2__p0_5 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> rec
; call f1 -> f2
; call f1 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=30
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

   LDA   g0+3
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f2__1
   AND   #$07
   STA   __local_f2__0
   LDA   g0
   SEC
   SBC   __zpabi_f2__p0_0
   STA   __local_f2__5
   LDA   g0+1
   SBC   __zpabi_f2__p0_1
   STA   __local_f2__4
   LDA   g0+2
   SBC   __zpabi_f2__p0_2
   STA   __local_f2__3
   LDA   g0+3
   SBC   __zpabi_f2__p0_3
   STA   __local_f2__2
   LDA   __local_f2__1
   SBC   __zpabi_f2__p0_4
   STA   __local_f2__1
   LDA   __local_f2__0
   SBC   __zpabi_f2__p0_5
   AND   #$07
   BNE   .thread@0
   LDA   __local_f2__1
   BNE   .thread@0
   LDA   __local_f2__2
   BNE   .thread@0
   LDA   __local_f2__3
   BNE   .thread@0
   LDA   __local_f2__4
   CMP   #$01
   BNE   .thread@0
   LDA   __local_f2__5
   CMP   #$80
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   ORA   #$64
   RTS

f1:
   SUBROUTINE

   ; prologue: 14 arg bytes, 0 local bytes
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
   ORA   g0+2
   ORA   g0+3
   BNE   .lb_skip@0
   JMP   .and_false@1
.lb_skip@0:
   LDA   g0
   STA   $8B
   LDA   g0+1
   STA   $8A
   LDA   g0+2
   STA   $89
   LDA   g0+3
   STA   $88
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   $87
   STA   $86
   LDA   $8B
   STA   HARGS
   LDA   $8A
   STA   HARGS+1
   LDA   $89
   STA   HARGS+2
   LDA   $88
   STA   HARGS+3
   LDA   $87
   STA   HARGS+4
   AND   #$07
   STA   HARGS+5
   LDY   #$05
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
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   STA   $8B
   LDA   HARGS+25
   STA   $8A
   LDA   HARGS+26
   STA   $89
   LDA   HARGS+27
   STA   $88
   LDA   HARGS+28
   STA   $87
   LDA   HARGS+29
   STA   $86
   LDA   $8B
   ORA   $8A
   ORA   $89
   ORA   $88
   ORA   $87
   ORA   $86
   BNE   .and_end@2
.and_false@1:
.and_end@2:
   LDY   #$03
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+1
   LDA   (FP),Y
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+2
   STA   $8A
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   $89
   STA   $88
   STA   $87
   STA   $86
   LDA   $8A
   STA   __zpabi_f2__p0_0
   LDA   #$00
   STA   __zpabi_f2__p0_1
   LDA   $89
   STA   __zpabi_f2__p0_2
   LDA   $88
   STA   __zpabi_f2__p0_3
   LDA   $87
   STA   __zpabi_f2__p0_4
   LDA   $86
   AND   #$07
   STA   __zpabi_f2__p0_5
   JSR   f2
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
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$05
   LDA   (FP),Y
   EOR   #$07
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$01
   STY   HARGS+8
   DEY
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$10
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

   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   BVC   .jcmp_novf@10
   EOR   #$80
.jcmp_novf@10:
   BMI   .and_false@3
   LDY   #$0C
   LDA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   STA   $85
   LDA   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   STA   $85
.and_end@4:
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   $83
   LDA   $85
   STA   $8B
   LDA   #$00
   STA   $8A
   LDA   $83
   STA   $89
   STA   $88
   STA   $87
   AND   #$07
   STA   $86
   LDY   #$0A
   LDA   (FP),Y
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $C0
   STA   $84
   LDY   #$0A
   LDA   (FP),Y
   STA   $85
   LDY   #$04
   LDA   (FP),Y
   EOR   $85
   STA   $85
   INY
   LDA   (FP),Y
   EOR   $84
   STA   $84
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   LDA   $8B
   CLC
   ADC   $85
   STA   $85
   LDA   #$00
   ADC   $84
   STA   $84
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$0D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0C
   LDA   (FP),Y
   ORA   $85
   STA   $81
   LDA   $84
   STA   $80
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   $80
   STA   $80
   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0D
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
   LDA   $85
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$0A
   LDA   (FP),Y
   EOR   (FP),Y
   STA   $81
   LDA   $C0
   EOR   $C0
   ORA   $81
   BEQ   .cmp_end@17
   LDA   #$01
.cmp_end@17:

   ; epilogue
   PHA
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
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$62
   LDY   #$01
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
   LDA   #$61
   INY
   STA   (SSP),Y
   LDA   #$12
   INY
   STA   (SSP),Y
   LDA   #$E5
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f0
   STA   $80
   EOR   g0
   STA   $C1
   LDA   #$00
   EOR   g0+1
   STA   $C0
   LDA   #$00
   EOR   g0+2
   STA   HARGS+2
   LDA   #$00
   EOR   g0+3
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

g0:
   DC.L  $0000003E
