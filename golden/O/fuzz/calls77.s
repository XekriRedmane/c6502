SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__zpabi_f3__p0_0	EQU	$80
__zpabi_f3__p0_1	EQU	$81
__zpabi_f3__p0_2	EQU	$82
__zpabi_f3__p0_3	EQU	$83
__zpabi_f3__pp_0	EQU	$84
__zpabi_f3__pp_1	EQU	$85
__local_f3__0	EQU	$86
__local_f3__1	EQU	$87
__local_f3__2	EQU	$88
__local_f3__box4	EQU	$89
__local_f4__1	EQU	$8C
__local_f4__2	EQU	$8D
__local_f4__pp	EQU	$8E
__local_f4__3	EQU	$8F

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__box4,__local_f3__4 indirect=false in_cycle=false param_regs=,,,,A,X,,
; def f4 params=__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__pp,__local_f4__3 indirect=false in_cycle=false param_regs=A,X
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> f4
; call f0 -> rec
; call f1 -> rec
; call f2 -> f3
; call f2 -> f4
; call f2 -> rec
; call f3 -> f4
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$1D weight=10
; zp g0 size=1 at=auto def=true init=$3D weight=4
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

f4:
   SUBROUTINE

   STA   __local_f4__pp
   STX   __local_f4__3
   LDA   g1
   ORA   #$01
   STA   HARGS+1
   LDA   g0
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+3
   SEC
   SBC   #$FF
   LDA   #$00
   SBC   #$FF
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f4__2
   LDY   #$01
   LDA   (__local_f4__pp),Y
   STA   __local_f4__1
   DEY
   LDA   (__local_f4__pp),Y
   CLC
   ADC   __local_f4__2
   BCC   .carry_inc@0
   INC   __local_f4__1
.carry_inc@0:
   LDY   #$00
   STA   (__local_f4__pp),Y
   LDA   __local_f4__1
   INY
   STA   (__local_f4__pp),Y
   LDA   #$E8
   RTS

f3:
   SUBROUTINE

   LDA   g0
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   STA   __local_f3__2
   LDY   #$01
   LDA   (__zpabi_f3__pp_0),Y
   STA   __local_f3__1
   DEY
   LDA   (__zpabi_f3__pp_0),Y
   CLC
   ADC   #$03
   BCC   .carry_inc@1
   INC   __local_f3__1
.carry_inc@1:
   LDY   #$00
   STA   (__zpabi_f3__pp_0),Y
   LDA   __local_f3__1
   INY
   STA   (__zpabi_f3__pp_0),Y
   LDA   __local_f3__2
   STA   __local_f3__box4
   LDA   #$00
   STA   __local_f3__box4+1
   LDX   #>__local_f3__box4
   LDA   #<__local_f3__box4
   JSR   f4
   LDA   #$01
   CLC
   ADC   g1
   STA   __local_f3__1
   LDA   #$00
   ADC   #$00
   STA   __local_f3__0
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__1
   STA   HARGS+8
   LDA   __local_f3__0
   STA   HARGS+9
   RTS

f2:
   SUBROUTINE

   ; prologue: 9 arg bytes, 9 local bytes, 5 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0A
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

   SEC
   LDA   #$00
   LDY   #$11
   SBC   (FP),Y
   STA   $C2
   LDA   #$00
   INY
   SBC   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   $C4
   LDY   #$0C
   LDA   (FP),Y
   AND   #$07
   TAX
   LDA   (FP),Y
   STA   $87
   LDA   $C4
   STA   $88
   CPX   #$00
   BEQ   .shift_done@9
.shift_loop@8:
   ASL   $87
   ROL   $88
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   $C2
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $86
   LDA   $87
   SEC
   SBC   $C2
   LDA   $88
   SBC   $86
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $C3
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $86
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $87
   INC   $86
   BNE   .inc_done@1
   INC   $87
.inc_done@1:
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $86
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $87
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y
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
   BVC   .jcmp_novf@13
   EOR   #$80
.jcmp_novf@13:
   BPL   .loop@0_break
   CLC
   LDA   FP
   ADC   #$06
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   LDX   $87
   LDA   $86
   JSR   f4
   CLC
   ADC   $C1
   CLC
   ADC   $C2
   STA   $C2
   INC   $C1
   BNE   .loop@0_start
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   $C2
   STA   $C0
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y
   LDA   #$FF
   SEC
   SBC   $C3
   STA   $8D
   LDA   #$7F
   SBC   #$00
   STA   $8C
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   $8B
   STA   $8A
   LDY   #$0F
   LDA   (FP),Y
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   LDA   #$01
   STA   $88
   LDA   #$00
   STA   $89
   CPX   #$00
   BEQ   .shift_done@17
.shift_loop@16:
   ASL   $88
   ROL   $89
   DEX
   BNE   .shift_loop@16
.shift_done@17:
   CLC
   LDA   FP
   ADC   #$08
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   LDA   $8D
   STA   __zpabi_f3__p0_0
   LDA   $8C
   STA   __zpabi_f3__p0_1
   LDA   $8B
   STA   __zpabi_f3__p0_2
   LDA   $8A
   STA   __zpabi_f3__p0_3
   LDA   $86
   STA   __zpabi_f3__pp_0
   LDA   $87
   STA   __zpabi_f3__pp_1
   LDX   $89
   LDA   $88
   JSR   f3
   LDA   $C0
   EOR   HARGS+8
   STA   $87
   LDY   #$08
   LDA   (FP),Y
   CLC
   ADC   $87
   CLC
   ADC   g1
   STA   g1
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $87
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $86
   LDA   $87
   SEC
   LDY   #$0C
   SBC   (FP),Y
   STA   $87
   LDA   $86
   INY
   SBC   (FP),Y
   STA   $86
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
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   $86
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$0E
   LDA   (FP),Y
   SEC
   SBC   (FP),Y

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
   CLC
   LDA   FP
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0A
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

   ; prologue: 8 arg bytes, 1 local bytes, 1 callee-saved bytes
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

   LDY   #$06
   LDA   (FP),Y
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   $83
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $81
   LDY   #$05
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   LDA   $83
   SBC   $83
   LDA   $80
   SBC   $80
   LDA   #$00
   ROL
   CLC
   ADC   $82
   STA   $80
   LDA   $81
   ADC   #$00
   STA   $81
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   AND   #$07
   STA   $C0
   INY
   LDA   (FP),Y
   LDA   #$FF
   STA   $80
   LDX   $C0
   CPX   #$00
   BEQ   .shift_done@21
.shift_loop@20:
   LSR   $80
   DEX
   BNE   .shift_loop@20
.shift_done@21:
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
   LDA   g1
   STA   $81
   LDA   #$00
   STA   $82
   LDX   $C0
   BEQ   .shift_done@23
.shift_loop@22:
   ASL   $81
   ROL   $82
   DEX
   BNE   .shift_loop@22
.shift_done@23:
   LDY   #$07
   LDA   (FP),Y
   BMI   .thread@0
   LDA   #$00
   BEQ   .vthread@0
.thread@0:
   LDA   #$00
.vthread@0:
   AND   #$07
   TAX
   BEQ   .shift_done@30
.shift_loop@29:
   LDA   $82
   ASL
   ROR   $82
   ROR   $81
   DEX
   BNE   .shift_loop@29
.shift_done@30:
   LDA   $82
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   HARGS+10
   STA   HARGS+11
   LDA   $81
   STA   HARGS+8
   LDA   $82
   STA   HARGS+9

   ; epilogue
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
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
   RTS

f0:
   SUBROUTINE

   ; prologue: 1 arg bytes, 6 local bytes, 2 callee-saved bytes
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

   LDA   g1
   BNE   .or_true@5
   LDA   #$03
   BNE   .jcmp_skip@33
   LDA   #$E8
   LDY   #$09
   CMP   (FP),Y
   BEQ   .or_true@5
.jcmp_skip@33:
   LDA   #$00
   STA   $C1
   STA   $C0
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   STA   $C1
   LDA   #$00
   STA   $C0
.or_end@6:
   ORA   #$00
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   STA   $81
   STA   $80
   LDA   #$E8
   LDY   #$03
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   $C1
   CLC
   SBC   #$01
   LDA   $C0
   SBC   #$00
   LDA   $81
   SBC   #$00
   LDA   $80
   SBC   #$00
   LDA   #$00
   ROL
   STA   $85
   LDA   g0
   STA   $80
   SEC
   LDA   #$00
   SBC   $80
   STA   $84
   LDA   #$00
   SBC   #$00
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
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $85
   LDY   #$01
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
   LDA   #$07
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
   JSR   f1
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   STA   $80
   LDA   #$E8
   EOR   $81
   STA   $83
   LDA   #$03
   EOR   $80
   STA   $82
   LDY   #$03
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   STA   $80
   LDA   $83
   CLC
   ADC   $81
   INY
   STA   (FP),Y
   LDA   $82
   ADC   $80
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
   JSR   f4
   STA   $80
   LDA   $C1
   EOR   $80
   STA   $81
   LDA   $C0
   STA   $80
   LDA   $81
   CLC
   LDY   #$05
   ADC   (FP),Y
   STA   $81
   LDA   $80
   INY
   ADC   (FP),Y
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
   LDA   #$01

   ; epilogue
   PHA
   LDY   #$01
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

main:
   SUBROUTINE

   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$36
   LDY   #$01
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

   LDA   #$3D
   STA   g0
   LDA   #$1D
   STA   g1
   RTS
