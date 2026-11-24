SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f5__0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__local_f5__1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__zpabi_f2__p0_2	EQU	$82
__zpabi_f2__p0_3	EQU	$83
__zpabi_f2__p1_0	EQU	$84
__zpabi_f2__p1_1	EQU	$85
__zpabi_f2__p1_2	EQU	$86
__zpabi_f2__p1_3	EQU	$87
__zpabi_f2__p2	EQU	$88
__zpabi_f2__p3_0	EQU	$89
__zpabi_f2__p3_1	EQU	$8A
__zpabi_f2__p3_2	EQU	$8B
__zpabi_f2__p3_3	EQU	$8C

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3,__zpabi_f2__p2,__zpabi_f2__p3_0,__zpabi_f2__p3_1,__zpabi_f2__p3_2,__zpabi_f2__p3_3 locals=__local_f2__0 indirect=false in_cycle=false
; def f3 params= locals= indirect=true in_cycle=false
; def f4 params= locals= indirect=true in_cycle=false
; def f5 params= locals=__local_f5__0,__local_f5__1 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f2
; call f0 -> f3
; call f1 -> f3
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$4E weight=47
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f5:
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

   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f5__0
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_f5__1
   CLC
   BCC   .carry_inc@0
   INC   __local_f5__1
.carry_inc@0:
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f5__0
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f5__1
   LDY   #$01
   STA   (DPTR),Y
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
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

   ; prologue: 10 arg bytes, 3 local bytes, 1 callee-saved bytes
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

   LDY   #$07
   LDA   (FP),Y
   AND   #$07
   TAX
   LDA   (FP),Y
   CPX   #$00
   BEQ   .shift_done@1
.shift_loop@0:
   ASL
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   STA   $C0
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<f5
   STA   $84
   LDA   #>f5
   STA   $85
   LDA   $C0
   AND   #$07
   TAX
   LDA   g0
   STA   $83
   CPX   #$00
   BEQ   .shift_done@3
.shift_loop@2:
   LSR   $83
   DEX
   BNE   .shift_loop@2
.shift_done@3:
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
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $83
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $84
   STA   DPTR
   LDA   $85
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $81
   LDY   #$02
   LDA   (FP),Y
   CLC
   ADC   $81
   STA   $80
   CLC
   ADC   g0
   STA   g0
   LDA   $80
   CLC
   SBC   g0
   LDA   #$00
   ROL
   LDX   #$00

   ; epilogue
   PHA
   TXA
   PHA
   DEY
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$0F
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
   TAX
   PLA
   RTS

f3:
   SUBROUTINE

   ; prologue: 5 arg bytes, 2 local bytes
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

   LDA   g0
   EOR   #$FF
   STA   $83
   LDA   #$00
   EOR   #$FF
   STA   $82
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   $81
   STA   $80
   LDY   #$05
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$02
   STA   (FP),Y
   LDA   #<f5
   STA   $85
   LDA   #>f5
   STA   $86
   LDA   $83
   SEC
   SBC   #$03
   LDA   $82
   SBC   #$00
   LDA   $81
   SBC   #$00
   LDA   $80
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $84
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   $83
   STA   $82
   CLC
   LDA   FP
   ADC   #$01
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$07
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $85
   STA   DPTR
   LDA   $86
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDY   #$05
   LDA   (FP),Y
   EOR   $80
   STA   $81
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   $81
   STA   $80
   CLC
   ADC   g0
   STA   g0
   LDA   $80
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   $80
   STA   HARGS
   LDY   #$06
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
   STA   $87
   LDA   HARGS+9
   STA   $86
   LDA   HARGS+10
   STA   $85
   LDA   HARGS+11
   STA   $84
   LDA   g0
   STA   $82
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   $81
   STA   $80
   LDA   $82
   LDY   #$06
   AND   (FP),Y
   STA   $83
   LDA   #$00
   INY
   AND   (FP),Y
   STA   $82
   LDA   $81
   INY
   AND   (FP),Y
   STA   $81
   LDA   $80
   INY
   AND   (FP),Y
   STA   $80
   LDA   $87
   SEC
   SBC   $83
   LDA   $86
   SBC   $82
   LDA   $85
   SBC   $81
   LDA   $84
   SBC   $80
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $80
   LDA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
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
   LDY   #$03
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f2:
   SUBROUTINE

   LDA   __zpabi_f2__p0_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f2__p0_1
   STA   HARGS+5
   LDA   __zpabi_f2__p0_2
   STA   HARGS+6
   LDA   __zpabi_f2__p0_3
   STA   HARGS+7
   LDA   __zpabi_f2__p3_0
   STA   HARGS
   LDA   __zpabi_f2__p3_1
   STA   HARGS+1
   LDA   __zpabi_f2__p3_2
   STA   HARGS+2
   LDA   __zpabi_f2__p3_3
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   LDA   __zpabi_f2__p1_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f2__p1_1
   STA   HARGS+5
   LDA   __zpabi_f2__p1_2
   STA   HARGS+6
   LDA   __zpabi_f2__p1_3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   RTS

f1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 12 local bytes, 10 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
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

   LDA   g0
   BNE   .and_false@2
   BEQ   .and_false@2
   LDA   #$01
   STA   $C9
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   $C9
.and_end@3:
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $C8
   SEC
   LDA   #$00
   SBC   $C9
   STA   $81
   LDA   #$00
   SBC   $C8
   ORA   $81
   BNE   .or_true@4
   LDA   $C9
   BNE   .or_true@4
   LDA   #$00
   STA   $C7
   STA   $C6
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   STA   $C7
   LDY   #$00
   STY   $C6
.or_end@5:
   CLC
   SBC   g0
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   $C0
   LDA   $C7
   AND   #$07
   TAX
   LDA   $C7
   STA   $83
   CPX   #$00
   BEQ   .shift_done@21
.shift_loop@20:
   ASL   $83
   DEX
   BNE   .shift_loop@20
.shift_done@21:
   LDA   $C9
   SEC
   SBC   $C0
   LDA   $C8
   SBC   #$00
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   STA   $82
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$05
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
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   STA   $80
   LDA   $C0
   EOR   $81
   STA   $C3
   LDA   #$00
   EOR   $80
   STA   $C2
   LDA   g0
   CLC
   ADC   $C3
   STA   g0
   LDA   $C9
   LDY   #$0B
   STA   (FP),Y
   LDA   $C8
   INY
   STA   (FP),Y
   LDA   #<f5
   STA   $C4
   LDA   #>f5
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
   BVC   .jcmp_novf@25
   EOR   #$80
.jcmp_novf@25:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   $C3
   SEC
   SBC   $C7
   LDA   $C2
   SBC   $C6
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   STA   $87
   LDA   g0
   SEC
   SBC   g0
   STA   $86
   LDA   $C9
   CLC
   ADC   #$02
   STA   $85
   LDA   $C8
   ADC   #$00
   STA   $84
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   $83
   STA   $82
   CLC
   LDA   FP
   ADC   #$0B
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $87
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
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
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$64
   SEC
   SBC   $C7
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
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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

f0:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes, 4 callee-saved bytes
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
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   $C3
   INY
   STA   (FP),Y

   LDA   g0
   CMP   g0
   BEQ   .cmp_true@29
   LDA   #$00
   BEQ   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   STA   $C2
   LDA   g0
   AND   g0
   STA   $8D
   AND   #$07
   TAX
   LDA   #$00
   STA   $C3
   CPX   #$00
   BEQ   .shift_done@32
.shift_loop@31:
   ASL   $C2
   ROL   $C3
   DEX
   BNE   .shift_loop@31
.shift_done@32:
   LDA   g0
   ORA   #$01
   STA   HARGS+1
   LDA   g0
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+2
   STA   $8D
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C2
   LDY   #$01
   STA   (SSP),Y
   LDA   $8D
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+8
   STA   $8E
   LDA   HARGS+9
   STA   $8D
   LDA   $C2
   EOR   $8E
   STA   $C1
   LDA   $C3
   EOR   $8D
   STA   $C0
   LDA   g0
   STA   $8E
   STA   $8D
   STA   __zpabi_f2__p0_0
   LDA   #$00
   STA   __zpabi_f2__p0_1
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   LDA   $C1
   STA   __zpabi_f2__p1_0
   LDA   $C0
   STA   __zpabi_f2__p1_1
   LDA   #$00
   STA   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_3
   LDA   #$01
   STA   __zpabi_f2__p2
   LDA   $8D
   STA   __zpabi_f2__p3_0
   LDA   #$00
   STA   __zpabi_f2__p3_1
   STA   __zpabi_f2__p3_2
   STA   __zpabi_f2__p3_3
   JSR   f2
   STA   $8E
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   $8D
   LDA   $C1
   EOR   $8E
   STA   $8E
   LDA   $C0
   EOR   $8D
   STA   $8D
   LDA   $8E
   CLC
   ADC   $8E
   STA   HARGS
   LDA   $8D
   ADC   $8D
   STA   HARGS+1
   LDA   g0
   CLC
   ADC   #$03
   STA   $8E
   LDA   #$00
   ADC   #$00
   STA   $8D
   LDA   $8E
   ORA   #$01
   STA   HARGS+2
   LDA   $8D
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   LDX   HARGS+5

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
   TAX
   PLA
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

   JSR   f0
   STA   $84
   STX   $83
   TXA
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   $82
   STA   $81
   LDA   g0
   STA   $80
   LDA   $84
   EOR   $80
   STA   $C1
   STX   $C0
   STA   HARGS
   LDA   $82
   STA   HARGS+2
   STA   HARGS+3
   STX   HARGS+1
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

   LDA   #$4E
   STA   g0
   RTS
