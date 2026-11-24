SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__zpabi_f4__p0_0	EQU	$80
__zpabi_f4__p0_1	EQU	$81
__zpabi_f4__p0_2	EQU	$82
__zpabi_f4__p0_3	EQU	$83
__zpabi_f4__p1	EQU	$84
__zpabi_f4__p3_0	EQU	$85
__zpabi_f4__p3_1	EQU	$86

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params= locals= indirect=false in_cycle=false
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p0_2,__zpabi_f4__p0_3,__zpabi_f4__p1,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p3_0,__zpabi_f4__p3_1 locals= indirect=false in_cycle=false param_regs=,,,,,A,X,,
; def f5 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f3
; call f0 -> f4
; call f1 -> rec
; call f2 -> f4
; call f2 -> f5
; call f2 -> rec
; call f3 -> f4
; call f3 -> f5
; call f3 -> rec
; call f5 -> rec
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
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$3A weight=24
; zp g0 size=1 at=auto def=true init=$99 weight=21
; runtime hargs_size=32 hargs_need=20
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

   ; prologue: 6 arg bytes, 3 local bytes, 3 callee-saved bytes
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
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   $C2
   STA   $C1
   STA   $C0
   LDA   #$64
   SEC
   LDY   #$09
   SBC   (FP),Y
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
   LDY   #$07
   LDA   (FP),Y
   ORA   #$64
   STA   $82
   INY
   LDA   (FP),Y
   STA   $81
   LDA   $C0
   BNE   .thread@0
   LDA   $C1
   BNE   .thread@0
   LDA   $C2
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$06
   LDA   (FP),Y
   INY
   CMP   (FP),Y
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   $80
   LDA   $82
   SEC
   SBC   $80
   LDA   $81
   SBC   #$00
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
   TAX
   PLA
   RTS

f4:
   SUBROUTINE

   LDA   #$FF
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f3:
   SUBROUTINE

   ; prologue: 4 arg bytes, 10 local bytes, 10 callee-saved bytes
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
   LDA   $C8
   INY
   STA   (FP),Y
   LDA   $C9
   INY
   STA   (FP),Y

   LDY   #$0D
   LDA   (FP),Y
   AND   #$07
   STA   HARGS+4
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
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
   JSR   asr32
   LDA   HARGS+5
   STA   $8A
   LDA   HARGS+6
   STA   $89
   LDA   HARGS+7
   STA   $88
   LDA   HARGS+8
   STA   $87
   LDA   $8A
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDA   $89
   INY
   SBC   (FP),Y
   LDA   $88
   INY
   SBC   (FP),Y
   LDA   $87
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $C9
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   $89
   STA   $87
   LDA   #$FF
   SEC
   LDY   #$0D
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
   BVC   .jcmp_novf@9
   EOR   #$80
.jcmp_novf@9:
   BPL   .or_true@1
   LDA   g0
   CMP   #$FF
   BCS   .or_true@1
   LDA   #$00
   STA   $C8
   STA   $C7
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   $C8
   LDA   #$00
   STA   $C7
.or_end@2:
   LDA   g1
   STA   $C5
   LDA   #$03
   BNE   .cmp_true@10
   LDA   #$E8
   CMP   $C5
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   $88
   LDA   $C9
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   $89
   INY
   SBC   (FP),Y
   LDA   $87
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   STA   $87
   LDA   #$00
   BNE   .cmp_true@14
   LDA   $88
   CMP   $87
   BNE   .cmp_true@14
   LDA   #$00
   BEQ   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   STA   $C6
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
   BVC   .jcmp_novf@17
   EOR   #$80
.jcmp_novf@17:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   $C8
   AND   g1
   STA   __zpabi_f4__p1
   LDA   g0
   STA   $87
   LDY   #$0D
   LDA   (FP),Y
   STA   __zpabi_f4__p0_0
   INY
   LDA   (FP),Y
   STA   __zpabi_f4__p0_1
   INY
   LDA   (FP),Y
   STA   __zpabi_f4__p0_2
   INY
   LDA   (FP),Y
   STA   __zpabi_f4__p0_3
   LDA   $87
   STA   __zpabi_f4__p3_0
   LDA   #$00
   STA   __zpabi_f4__p3_1
   LDX   #$00
   LDA   #$FF
   JSR   f4
   LDA   HARGS+8
   STA   $8A
   LDA   HARGS+9
   STA   $89
   LDA   HARGS+10
   STA   $8C
   LDA   HARGS+11
   STA   $8B
   LDA   $C0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $88
   STA   $87
   LDA   $8A
   CLC
   ADC   $C1
   STA   $8A
   LDA   $89
   ADC   $C0
   STA   $89
   LDA   $8C
   ADC   $88
   STA   $88
   LDA   $8B
   ADC   $87
   STA   $87
   LDA   $C5
   CLC
   ADC   $8A
   STA   $C5
   LDA   $C4
   ADC   $89
   STA   $C4
   LDA   $C3
   ADC   $88
   STA   $C3
   LDA   $C2
   ADC   $87
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
   STA   $87
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C9
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$FF
   INY
   STA   (SSP),Y
   LDA   $C6
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f5
   STA   $87
   LDA   $C9
   EOR   $87
   STA   $87
   CLC
   ADC   g0
   STA   g0
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
   LDA   $C6
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   CLC
   ADC   $C8
   STA   $87
   TXA
   ADC   $C7
   STA   $88
   LDA   $87
   AND   $87
   STA   $87
   LDA   $88
   AND   $88
   STA   $88
   LDA   $87
   ORA   $C5
   STA   $87
   LDA   $88
   ORA   $C4
   TAX
   LDA   $87

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
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   CLC
   LDA   FP
   ADC   #$10
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

   ; prologue: 8 arg bytes, 3 local bytes, 3 callee-saved bytes
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

   LDA   #$03
   LDY   #$06
   EOR   (FP),Y
   STA   $88
   LDA   #$00
   INY
   EOR   (FP),Y
   STA   $87
   ORA   $88
   BEQ   .lnot_true@20
   LDA   #$00
   BEQ   .lnot_end@21
.lnot_true@20:
   LDA   #$01
.lnot_end@21:
   STA   $C2
   LDA   g0
   AND   #$07
   STA   HARGS+4
   LDY   #$08
   LDA   (FP),Y
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
   JSR   asl32
   LDA   HARGS+5
   STA   HARGS
   LDA   HARGS+6
   STA   HARGS+1
   LDA   HARGS+7
   STA   HARGS+2
   LDA   HARGS+8
   STA   HARGS+3
   LDA   g0
   ORA   #$01
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   STA   $C1
   LDA   HARGS+13
   STA   $88
   LDA   g1
   SEC
   SBC   $C1
   LDA   #$00
   SBC   $88
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   STA   $C0
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $87
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $88
   INC   $87
   BNE   .inc_done@2
   INC   $88
.inc_done@2:
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $87
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $88
   LDY   #$01
   STA   (DPTR),Y
   LDA   g1
   SEC
   SBC   $C0
   STA   __zpabi_f4__p1
   LDA   g0
   STA   $87
   LDA   $C2
   STA   __zpabi_f4__p0_0
   LDA   #$00
   STA   __zpabi_f4__p0_1
   STA   __zpabi_f4__p0_2
   STA   __zpabi_f4__p0_3
   LDY   #$06
   LDA   (FP),Y
   STA   __zpabi_f4__p3_0
   LDA   #$00
   STA   __zpabi_f4__p3_1
   LDX   #$00
   LDA   $87
   JSR   f4
   LDA   $C2
   EOR   HARGS+8
   STA   $87
   CLC
   ADC   g1
   STA   g1
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   HARGS+2
   LDY   #$0C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   HARGS+3
   LDY   #$06
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+4
   STA   $88
   LDA   HARGS+5
   STA   $87
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$02
   LDY   #$01
   STA   (SSP),Y
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f5
   STA   $87
   LDA   $C1
   EOR   $87
   CLC
   ADC   g1
   STA   g1
   LDA   g0
   STA   $87
   SEC
   LDA   #$00
   SBC   $87
   STA   $88
   LDA   #$00
   SBC   #$00
   STA   $87
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
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   g0
   STA   $8A
   LDA   g1
   STA   $89
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   $88
   STA   $87
   LDY   #$08
   LDA   (FP),Y
   SEC
   SBC   $89
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   $88
   INY
   LDA   (FP),Y
   SBC   $87
   LDA   #$00
   ROL
   SEC
   SBC   $8A
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
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
   CLC
   LDA   FP
   ADC   #$0D
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
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   STA   $80
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   $80
   AND   #$07
   TAX
   LDY   #$04
   LDA   (FP),Y
   STA   $80
   INY
   LDA   (FP),Y
   STA   $81
   CPX   #$00
   BEQ   .shift_done@27
.shift_loop@26:
   ASL   $80
   ROL   $81
   DEX
   BNE   .shift_loop@26
.shift_done@27:
   LDA   $80
   CLC
   LDY   #$04
   SBC   (FP),Y
   LDA   $81
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $80
   LDA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   STA   HARGS+10
   STA   HARGS+11
   LDA   $80
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9

   ; epilogue
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
   RTS

f0:
   SUBROUTINE

   ; prologue: 12 arg bytes, 1 local bytes, 1 callee-saved bytes
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
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   $8A
   LDA   HARGS+9
   STA   $89
   LDA   HARGS+10
   STA   $88
   LDA   HARGS+11
   STA   $87
   LDA   $8A
   EOR   #$FF
   STA   $C0
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
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f3
   STA   $87
   LDA   $C0
   EOR   $87
   STA   $C0
   LDA   #$00
   SEC
   SBC   $C0
   LDA   #$00
   ROL
   STA   __zpabi_f4__p1
   LDA   #$07
   CLC
   SBC   g1
   LDA   #$00
   ROL
   STA   __zpabi_f4__p3_0
   LDY   #$08
   LDA   (FP),Y
   STA   __zpabi_f4__p0_0
   INY
   LDA   (FP),Y
   STA   __zpabi_f4__p0_1
   INY
   LDA   (FP),Y
   STA   __zpabi_f4__p0_2
   INY
   LDA   (FP),Y
   STA   __zpabi_f4__p0_3
   LDA   #$00
   STA   __zpabi_f4__p3_1
   LDX   #$00
   LDA   #$64
   JSR   f4
   LDA   $C0
   EOR   HARGS+8
   CLC
   ADC   g0
   STA   g0
   LDY   #$0F
   LDA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@30
   LDA   #$01
.cmp_end@30:

   ; epilogue
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$0F
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

   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$A6
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$34
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$8A
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
   STA   $81
   LDA   g0
   EOR   g1
   STA   $80
   LDA   $81
   EOR   $80
   LDX   #$00
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$99
   STA   g0
   LDA   #$3A
   STA   g1
   RTS
