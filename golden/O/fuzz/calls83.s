SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
__zpabi_f0__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f0__pp_0	EQU	$84
__zpabi_f0__pp_1	EQU	$85
__local_main__0	EQU	$86
__local_main__2	EQU	$88
__local_main__mbox	EQU	$8A
__local_f0__0	EQU	$8C
__local_f0__1	EQU	$8D
__local_f0__2	EQU	$8E

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2 indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params= locals= indirect=false in_cycle=false
; def f4 params= locals= indirect=false in_cycle=false
; def f5 params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__mbox,__local_main__5 indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f1 -> f2
; call f1 -> f5
; call f1 -> rec
; call f2 -> f3
; call f2 -> f5
; call f2 -> rec
; call f3 -> f5
; call f4 -> f5
; call f4 -> rec
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
; zp g0 size=1 at=auto def=true init=$20 weight=21
; zp g1 size=1 at=auto def=true init=$08 weight=18
; runtime hargs_size=32 hargs_need=24
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

   ; prologue: 22 arg bytes, 0 local bytes
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

   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDY   #$0B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   STA   $81
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   $81
.or_end@2:
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   #$00
   STA   $80
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   g0
   LDX   #$FF
   SEC
.div_loop@1:
   INX
   SBC   #$07
   BCS   .div_loop@1
   STX   $80
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
   STA   $80
   LDA   #$07
   CLC
   ADC   $80
   STA   $80
   AND   #$7F
   STA   HARGS+4
   LDY   #$0B
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
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
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

f4:
   SUBROUTINE

   ; prologue: 3 arg bytes, 4 local bytes, 2 callee-saved bytes
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

   LDA   g0
   STA   $80
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$08
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   SEC
   LDA   #$00
   SBC   $81
   STA   $C1
   LDA   g1
   ORA   #$01
   STA   HARGS+1
   LDA   g0
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+2
   STA   $81
   LDA   $C1
   AND   #$7F
   STA   $80
   LDA   $81
   SEC
   SBC   $80
   LDA   #$00
   ROL
   STA   $C0
   LDA   $C1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   $81
   LDA   $C1
   LDY   #$03
   STA   (FP),Y
   LDA   $81
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   SBC   $C1
   LDA   #$00
   SBC   $81
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   STA   $8B
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   $8A
   STA   $89
   STA   $88
   STA   $87
   STA   $86
   STA   $85
   LDA   $C1
   BEQ   .lnot_true@7
   LDA   #$00
   BEQ   .lnot_end@8
.lnot_true@7:
   LDA   #$01
.lnot_end@8:
   STA   $84
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   $83
   STA   $82
   CLC
   LDA   FP
   ADC   #$03
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $8B
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $8A
   INY
   STA   (SSP),Y
   LDA   $89
   INY
   STA   (SSP),Y
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   $87
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
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   #$64
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
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f5
   LDA   $C0
   EOR   HARGS+8
   STA   $81
   LDY   #$03
   LDA   (FP),Y
   CLC
   ADC   $81
   CLC
   ADC   g0
   STA   g0
   STA   $80
   LDA   #$FF
   ORA   $80
   STA   $81
   LDA   #$7F
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
   LDA   g0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23

   ; epilogue
   INY
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   CLC
   LDA   FP
   ADC   #$09
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
   RTS

f3:
   SUBROUTINE

   ; prologue: 0 arg bytes, 3 local bytes, 1 callee-saved bytes
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

   SEC
   BCC   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@12:
   LDA   #$01
.cmp_end@11:
   AND   #$7F
   STA   $C0
   LDA   #$01
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   $C0
   SEC
   SBC   g0
   STA   $8A
   LDA   #$00
   SBC   #$00
   STA   $89
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $88
   STA   $87
   STA   $86
   STA   $85
   STA   $84
   STA   $83
   LDA   g1
   STA   $82
   CLC
   LDA   FP
   ADC   #$02
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $8A
   LDY   #$01
   STA   (SSP),Y
   LDA   $89
   INY
   STA   (SSP),Y
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   $87
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
   LDA   $83
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
   LDA   $82
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
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f5
   LDA   $C0

   ; epilogue
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$04
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

   ; prologue: 14 arg bytes, 4 local bytes, 2 callee-saved bytes
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

   LDA   g1
   STA   $82
   STA   $81
   LDY   #$09
   LDA   (FP),Y
   STA   $C0
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $80
   LDA   #$00
   BNE   .thread@0
   LDA   $81
   CMP   $80
   BEQ   .cmp_true@15
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   STA   $C1
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $80
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $81
   LDA   $80
   CLC
   ADC   $C1
   STA   $80
   LDA   $81
   ADC   #$00
   STA   $81
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$01
   STA   (DPTR),Y
   LDA   $82
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $8E
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $8D
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $8C
   STA   $8B
   STA   $8A
   STA   $89
   STA   $88
   STA   $87
   LDA   g0
   STA   $82
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   $81
   STA   $80
   LDA   $82
   LDY   #$09
   EOR   (FP),Y
   STA   $86
   LDA   #$00
   INY
   EOR   (FP),Y
   STA   $85
   LDA   $81
   INY
   EOR   (FP),Y
   STA   $84
   LDA   $80
   INY
   EOR   (FP),Y
   STA   $83
   LDA   g1
   STA   $82
   CLC
   LDA   FP
   ADC   #$03
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $8E
   LDY   #$01
   STA   (SSP),Y
   LDA   $8D
   INY
   STA   (SSP),Y
   LDA   $8C
   INY
   STA   (SSP),Y
   LDA   $8B
   INY
   STA   (SSP),Y
   LDA   $8A
   INY
   STA   (SSP),Y
   LDA   $89
   INY
   STA   (SSP),Y
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   $87
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
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
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
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f5
   LDA   $C0
   EOR   HARGS+8
   STA   $81
   LDY   #$03
   LDA   (FP),Y
   STA   $80
   CLC
   ADC   $81
   STA   $C0
   CLC
   ADC   g0
   STA   g0
   JSR   f3
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
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDX   #$00
   LDA   $C1

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
   ADC   #$14
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

f1:
   SUBROUTINE

   ; prologue: 10 arg bytes, 11 local bytes, 7 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0C
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

   LDA   g1
   STA   $80
   LDY   #$0E
   LDA   (FP),Y
   SEC
   SBC   $80
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .jcmp_novf@22
   EOR   #$80
.jcmp_novf@22:
   BPL   .or_true@5
   LDA   g1
   AND   #$07
   TAX
   BEQ   .shift_done@24
.shift_loop@23:
   LSR   $80
   DEX
   BNE   .shift_loop@23
.shift_done@24:
   LDA   $80
   BNE   .or_true@5
   LDA   #$00
   STA   $C0
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   STA   $C0
.or_end@6:
   LDY   #$14
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $82
   LDA   g0
   BNE   .or_true@7
   LDA   g1
   BNE   .or_true@7
   LDA   #$00
   STA   $81
   JMP   .vthread@0
.or_true@7:
   LDA   #$01
   STA   $81
   LDA   #$00
   BNE   .cmp_true@26
.vthread@0:
   LDA   $82
   CMP   $81
   BNE   .cmp_true@26
   LDA   #$00
   BEQ   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   STA   $C6
   LDA   g1
   STA   $80
   LDY   #$0E
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   LDA   $80
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   LDA   $81
   SEC
   SBC   $C0
   LDA   $80
   SBC   #$00
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $C5
   LDA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   STA   $C4
   STA   $C3
   LDA   $C0
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   $C5
   EOR   #$FF
   STA   $89
   LDA   #$00
   EOR   #$FF
   STA   $88
   LDA   $C4
   EOR   #$FF
   STA   $87
   LDA   $C3
   EOR   #$FF
   STA   $86
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   $85
   STA   $84
   STA   $83
   STA   $82
   CLC
   LDA   FP
   ADC   #$08
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $89
   LDY   #$01
   STA   (SSP),Y
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   $87
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
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $C5
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   LDA   $C3
   INY
   STA   (SSP),Y
   LDA   #$18
   INY
   STA   (SSP),Y
   LDA   #$FC
   INY
   STA   (SSP),Y
   LDA   #$FF
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f5
   LDA   $C0
   EOR   HARGS+8
   STA   $81
   LDY   #$08
   LDA   (FP),Y
   CLC
   ADC   $81
   STA   $C2
   CLC
   ADC   g0
   STA   g0
   LDA   $C2
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@0_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@34
   EOR   #$80
.jcmp_novf@34:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDY   #$10
   LDA   (FP),Y
   ASL
   STA   $8A
   INY
   LDA   (FP),Y
   ROL
   ASL   $8A
   ROL
   ASL   $8A
   ROL
   STA   $8B
   LDA   $C2
   SEC
   SBC   $C2
   LDA   #$00
   ROL
   STA   $89
   LDA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   $88
   STA   $87
   STA   $86
   STA   $85
   STA   $84
   STA   $83
   LDA   #$03
   SEC
   SBC   $C5
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   $C4
   LDA   #$00
   SBC   $C3
   BVC   .cmp_novf@37
   EOR   #$80
.cmp_novf@37:
   ASL
   LDA   #$00
   ROL
   STA   $82
   CLC
   LDA   FP
   ADC   #$0A
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $8A
   LDY   #$01
   STA   (SSP),Y
   LDA   $8B
   INY
   STA   (SSP),Y
   LDA   $89
   INY
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
   LDA   $86
   INY
   STA   (SSP),Y
   LDA   $85
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
   INY
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
   JSR   f2
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
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   $C2
   CLC
   ADC   g1
   AND   #$7F
   STA   g1
   LDY   #$14
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   $C5
   ORA   $C4
   ORA   $C3
   BNE   .or_true@9
   LDA   #$00
   STA   $81
   STA   $80
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   STA   $81
   LDA   #$00
   STA   $80
.or_end@10:
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
   ADC   $C6
   STA   $81
   LDA   g0
   STA   $80
   LDA   $81
   BMI   .sx_neg@38
   LDA   #$00
   BEQ   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   STA   HARGS+3
   LDA   $80
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   $81
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+4
   STA   $82
   LDA   HARGS+5
   STA   $81
   LDY   #$0E
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   SEC
   SBC   $82
   LDA   #$00
   SBC   $81
   BVC   .cmp_novf@41
   EOR   #$80
.cmp_novf@41:
   ASL
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
   INY
   LDA   (FP),Y
   STA   $C6
   CLC
   LDA   FP
   ADC   #$17
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0C
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

   LDA   g1
   SEC
   SBC   __zpabi_f0__p0_0
   LDA   #$00
   SBC   __zpabi_f0__p0_1
   LDA   #$00
   SBC   __zpabi_f0__p0_2
   LDA   #$00
   SBC   __zpabi_f0__p0_3
   BVC   .cmp_novf@42
   EOR   #$80
.cmp_novf@42:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f0__0
   LDA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   STA   HARGS+2
   STA   HARGS+3
   LDA   g1
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f0__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   AND   #$7F
   STA   __local_f0__2
   LDA   g1
   BNE   .f0@jthread@0
   LDA   g0
   BEQ   .and_false@11
.f0@jthread@0:
   LDA   __local_f0__2
   CLC
   ADC   #$E8
   STA   __local_f0__1
   LDA   #$00
   ADC   #$03
   ORA   __local_f0__1
   BNE   .and_end@12
.and_false@11:
.and_end@12:
   LDA   #$00
   CMP   __zpabi_f0__p0_3
   BNE   .cmp_end@46
   CMP   __zpabi_f0__p0_2
   BNE   .cmp_end@46
   CMP   __zpabi_f0__p0_1
   BNE   .cmp_end@46
   LDA   __local_f0__2
   CMP   __zpabi_f0__p0_0
   BEQ   .cmp_true@45
   LDA   #$00
   BEQ   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   STA   __local_f0__2
   LDY   #$01
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__1
   DEY
   LDA   (__zpabi_f0__pp_0),Y
   CLC
   ADC   __local_f0__2
   BCC   .carry_inc@0
   INC   __local_f0__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__1
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDA   g0
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$01
   STA   __zpabi_f0__p0_0
   STA   __zpabi_f0__p0_1
   LDA   #$00
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   JSR   f0
   STA   __local_main__2
   LDA   g0
   EOR   g1
   STA   __local_main__0
   LDA   __local_main__2
   EOR   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$20
   STA   g0
   LDA   #$08
   STA   g1
   RTS
