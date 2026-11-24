SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f5__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__local_f5__1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__local_f5__2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__local_f5__3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__local_f5__4	EQU	$84
__zpabi_f1__pp_0	EQU	$84
__local_f5__5	EQU	$85
__zpabi_f1__pp_1	EQU	$85
__local_f5__6	EQU	$86
__local_f1__1	EQU	$87
__local_f5__7	EQU	$87

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false carry=true
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params= locals= indirect=true in_cycle=false
; def f4 params= locals= indirect=true in_cycle=false
; def f5 params= locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3,__local_f5__4,__local_f5__5,__local_f5__6,__local_f5__7 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f2 -> f4
; call f3 -> f4
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$2A,$00 weight=9
; runtime hargs_size=32 hargs_need=26
; @zp-link-meta-end

f5:
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
   STA   __local_f5__4
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f5__3
   STA   __local_f5__2
   STA   __local_f5__1
   STA   __local_f5__0
   LDY   #$03
   LDA   (FP),Y
   STA   __local_f5__5
   LDA   __local_f5__3
   AND   #$03
   STA   __local_f5__0
   LDA   g1
   SEC
   SBC   __local_f5__5
   STA   __local_f5__5
   LDA   g1+1
   SBC   __local_f5__4
   STA   __local_f5__4
   LDA   g1+2
   SBC   __local_f5__3
   STA   __local_f5__3
   LDA   g1+3
   SBC   __local_f5__2
   STA   __local_f5__2
   LDA   g1+4
   SBC   __local_f5__1
   STA   __local_f5__1
   LDA   g1+5
   SBC   __local_f5__0
   LDX   __local_f5__5
   STX   HARGS
   LDX   __local_f5__4
   STX   HARGS+1
   LDX   __local_f5__3
   STX   HARGS+2
   LDX   __local_f5__2
   STX   HARGS+3
   LDX   __local_f5__1
   STX   HARGS+4
   AND   #$03
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$00
   STY   HARGS+8
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDX   HARGS+17
   STX   __local_f5__4
   LDX   HARGS+18
   STX   __local_f5__3
   LDX   HARGS+19
   STX   __local_f5__2
   LDX   HARGS+20
   STX   __local_f5__1
   LDX   HARGS+21
   STX   __local_f5__0
   STA   __local_f5__7
   LDA   __local_f5__4
   STA   __local_f5__6
   LDA   __local_f5__3
   STA   __local_f5__5
   LDA   __local_f5__2
   STA   __local_f5__4
   LDA   __local_f5__1
   STA   __local_f5__3
   LDA   __local_f5__0
   AND   #$03
   STA   __local_f5__2
   LDA   __local_f5__7
   ORA   #$01
   STA   HARGS+8
   LDA   __local_f5__6
   STA   HARGS+9
   LDA   __local_f5__5
   STA   HARGS+10
   LDA   __local_f5__4
   STA   HARGS+11
   LDA   __local_f5__3
   STA   HARGS+12
   LDA   __local_f5__2
   STA   HARGS+13
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   LDY   #$00
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   LDX   HARGS+25

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

f4:
   SUBROUTINE

   ; prologue: 3 arg bytes, 1 local bytes, 1 callee-saved bytes
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

   LDA   g1
   STA   $88
   LDA   g1+1
   STA   $87
   LDA   g1+2
   STA   $86
   LDA   g1+3
   STA   $85
   LDA   g1
   STA   $82
   LDA   g1+1
   STA   $81
   LDA   g1
   SEC
   SBC   #$07
   LDA   g1+1
   SBC   #$00
   LDA   g1+2
   SBC   #$00
   LDA   g1+3
   SBC   #$00
   LDA   g1+4
   SBC   #$00
   LDA   g1+5
   SBC   #$00
   LDA   #$00
   ROL
   STA   $80
   LDA   $82
   EOR   $80
   STA   $84
   ORA   $81
   BEQ   .and_false@0
   LDA   #$01
   STA   $83
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   $83
.and_end@1:
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   $85
   BNE   .cmp_true@4
   LDA   $86
   BNE   .cmp_true@4
   LDA   $87
   CMP   $80
   BNE   .cmp_true@4
   LDA   $88
   CMP   $81
   BNE   .cmp_true@4
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   $80
   LDA   #$00
   BNE   .cmp_true@7
   LDA   $83
   CMP   $80
   BNE   .cmp_true@7
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   STA   $C0
   LDY   #$05
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $80
   LDY   #$05
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDA   $80
   CLC
   ADC   $C0
   BCC   .carry_inc@0
   INC   $82
   CLC
.carry_inc@0:
   ADC   #$FF
   STA   $81
   LDA   $82
   ADC   #$7F
   STA   $80
   LDY   #$05
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f5
   STA   $81
   LDA   #>f5
   STA   $82
   LDY   #$05
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $80
   LDY   #$05
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   LDA   $80
   AND   $84
   STA   $80
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   STA   (SSP),Y
   LDA   $81
   STA   DPTR
   LDA   $82
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $86
   CLC
   ADC   g1
   STA   $85
   LDA   g1+1
   ADC   #$00
   STA   $84
   LDA   g1+2
   ADC   #$00
   STA   $83
   LDA   g1+3
   ADC   #$00
   STA   $82
   LDA   g1+4
   ADC   #$00
   STA   $81
   LDA   g1+5
   ADC   #$00
   STA   $80
   LDA   $85
   STA   g1
   LDA   $84
   STA   g1+1
   LDA   $83
   STA   g1+2
   LDA   $82
   STA   g1+3
   LDA   $81
   STA   g1+4
   LDA   $80
   AND   #$03
   STA   g1+5
   LDA   $86
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11

   ; epilogue
   INY
   LDA   (FP),Y
   STA   $C0
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
   RTS

f3:
   SUBROUTINE

   ; prologue: 7 arg bytes, 6 local bytes, 4 callee-saved bytes
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

   LDA   g1+5
   BNE   .cmp_true@10
   LDA   g1+4
   BNE   .cmp_true@10
   LDA   g1+3
   BNE   .cmp_true@10
   LDA   g1+2
   BNE   .cmp_true@10
   LDA   g1+1
   LDY   #$0E
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   g1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   HARGS+2
   LDY   #$0D
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $C1
   LDA   HARGS+5
   STA   $C0
   LDA   #<f5
   STA   $81
   LDA   #>f5
   STA   $82
   LDA   #$00
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_end@14
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@14
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$0A
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$09
   CMP   (FP),Y
   BEQ   .cmp_true@13
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   $80
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   STA   DPTR
   LDA   $82
   STA   DPTR+1
   JSR   icall
   STA   $81
   STX   $80
   LDA   $C1
   EOR   $81
   STA   $C3
   LDA   $C0
   EOR   $80
   STA   $C0
   LDA   g1
   CLC
   ADC   $C3
   STA   $85
   LDA   g1+1
   ADC   $C0
   STA   $84
   LDA   g1+2
   ADC   #$00
   STA   $83
   LDA   g1+3
   ADC   #$00
   STA   $82
   LDA   g1+4
   ADC   #$00
   STA   $81
   LDA   g1+5
   ADC   #$00
   STA   $80
   LDA   $85
   STA   g1
   LDA   $84
   STA   g1+1
   LDA   $83
   STA   g1+2
   LDA   $82
   STA   g1+3
   LDA   $81
   STA   g1+4
   LDA   $80
   AND   #$03
   STA   g1+5
   LDA   $C3
   LDY   #$05
   STA   (FP),Y
   LDA   $C0
   INY
   STA   (FP),Y
   LDA   #$00
   STA   $C2
   LDY   #$00
   STY   $C1
.loop@0_start:
   LDA   $C2
   SEC
   SBC   #$03
   LDA   $C1
   SBC   #$00
   BVC   .jcmp_novf@16
   EOR   #$80
.jcmp_novf@16:
   BPL   .loop@0_break
   LDA   #$64
   CLC
   SBC   $C3
   LDA   #$00
   SBC   $C0
   LDA   #$00
   ROL
   STA   $82
   CLC
   LDA   FP
   ADC   #$05
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $82
   LDY   #$01
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f4
   LDA   HARGS+8
   STA   $80
   LDA   HARGS+9
   STA   $81
   LDA   $80
   CLC
   ADC   $C2
   STA   $80
   LDA   $81
   ADC   $C1
   STA   $81
   SEC
   BCS   .carry_inc@1
   DEC   $81
.carry_inc@1:
   LDA   $C3
   CLC
   ADC   $80
   STA   $80
   LDA   $C0
   ADC   $81
   STA   $81
   STA   $C0
   LDA   $80
   SEC
   STA   $C3
   BCS   .carry_inc@2
   DEC   $C0
.carry_inc@2:
   INC   $C2
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C1
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$05
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   STA   $80
   LDA   $C3
   CLC
   ADC   $81
   STA   $81
   LDA   $C0
   ADC   $80
   STA   $80
   LDA   g1
   CLC
   ADC   $81
   STA   $85
   LDA   g1+1
   ADC   $80
   STA   $84
   LDA   g1+2
   ADC   #$00
   STA   $83
   LDA   g1+3
   ADC   #$00
   STA   $82
   LDA   g1+4
   ADC   #$00
   STA   $81
   LDA   g1+5
   ADC   #$00
   STA   $80
   LDA   $85
   STA   g1
   LDA   $84
   STA   g1+1
   LDA   $83
   STA   g1+2
   LDA   $82
   STA   g1+3
   LDA   $81
   STA   g1+4
   LDA   $80
   AND   #$03
   STA   g1+5
   LDY   #$0F
   LDA   (FP),Y

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
   ADC   #$0F
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

f2:
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

   LDA   g1
   STA   $C0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$07
   CLC
   SBC   $C0
   LDA   #$00
   ROL
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
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $82
   LDY   #$01
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f4
   LDA   $C0
   EOR   HARGS+8
   STA   $81
   LDY   #$02
   LDA   (FP),Y
   CLC
   ADC   $81
   STA   $84
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   $83
   STA   $82
   STA   $81
   LDA   $84
   STA   $85
   LDA   $83
   AND   #$03
   STA   $80
   LDA   $85
   EOR   g1
   STA   $87
   LDA   #$00
   EOR   g1+1
   STA   $86
   LDA   $83
   EOR   g1+2
   STA   $85
   LDA   $82
   EOR   g1+3
   STA   $84
   LDA   $81
   EOR   g1+4
   STA   $83
   LDA   $80
   EOR   g1+5
   STA   $82
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   $81
   CLC
   SBC   $87
   LDA   $80
   SBC   $86
   LDA   #$00
   SBC   $85
   LDA   #$00
   SBC   $84
   LDA   #$00
   SBC   $83
   LDA   #$00
   SBC   $82
   LDA   #$00
   ROL

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

f1:
   SUBROUTINE

   LDY   #$01
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   LDA   #$E8
   SEC
   SBC   g0
   ORA   g0
   CLC
   DEY
   ADC   (__zpabi_f1__pp_0),Y
   BCC   .carry_inc@3
   INC   __local_f1__1
.carry_inc@3:
   LDY   #$00
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   INY
   STA   (__zpabi_f1__pp_0),Y
   DEY
   LDA   (__zpabi_f1__pp_0),Y
   STA   HARGS+2
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   HARGS+3
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+5
   ORA   HARGS+4
   BEQ   .cmp_end@19
   SEC
   RTS
.cmp_end@19:
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   ; prologue: 4 arg bytes, 17 local bytes, 17 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$13
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$12
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
   LDA   $CA
   INY
   STA   (FP),Y
   LDA   $CB
   INY
   STA   (FP),Y
   LDA   $CC
   INY
   STA   (FP),Y
   LDA   $CD
   INY
   STA   (FP),Y
   LDA   $CE
   INY
   STA   (FP),Y
   LDA   $CF
   INY
   STA   (FP),Y
   LDA   $D0
   INY
   STA   (FP),Y

   LDY   #$17
   LDA   (FP),Y
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   $C1
   LDY   #$15
   LDA   (FP),Y
   STA   $C7
   LDA   #$7F
   INY
   AND   (FP),Y
   STA   $C6
   LDY   #$14
   LDA   (FP),Y
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   $C7
   AND   $C7
   STA   $83
   LDA   $C6
   AND   $C6
   STA   $82
   LDA   #$00
   STA   $81
   STA   $80
   LDA   $83
   ORA   #$01
   STA   HARGS+4
   LDA   $82
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$14
   LDA   (FP),Y
   STA   HARGS
   JSR   udivmod32
   LDA   HARGS+12
   STA   $D0
   LDA   HARGS+13
   STA   $CF
   LDA   HARGS+14
   STA   $CE
   LDA   HARGS+15
   STA   $CD
   LDY   #$14
   LDA   (FP),Y
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $CC
   STA   $CB
   STA   $CA
   LDA   #<f5
   STA   $C8
   LDA   #>f5
   STA   $C9
   LDA   #$00
   STA   $C5
   STA   $C4
   STA   $C3
   LDY   #$00
   STY   $C2
.loop@1_start:
   LDA   $C3
   SEC
   SBC   #$03
   LDA   $C2
   SBC   #$00
   BVC   .jcmp_novf@26
   EOR   #$80
.jcmp_novf@26:
   BPL   .loop@1_break
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDA   $C8
   STA   DPTR
   LDA   $C9
   STA   DPTR+1
   JSR   icall
   CLC
   ADC   $C3
   STA   $83
   TXA
   ADC   $C2
   STA   $82
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   $81
   STA   $80
   LDA   $C7
   CLC
   ADC   $83
   STA   $C7
   LDA   $C6
   ADC   $82
   STA   $C6
   LDA   $C5
   ADC   $81
   STA   $C5
   LDA   $C4
   ADC   $80
   STA   $C4
   INC   $C3
   BNE   .loop@1_start
   INC   $C2
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$14
   LDA   (FP),Y
   ORA   $D0
   AND   #$07
   TAX
   LDY   #$17
   LDA   (FP),Y
   STA   $C0
   CPX   #$00
   BEQ   .shift_done@30
.shift_loop@29:
   LDA   $C1
   ASL
   ROR   $C1
   ROR   $C0
   DEX
   BNE   .shift_loop@29
.shift_done@30:
   LDA   $C1
   ORA   $C0
   BEQ   .cmp_end@31
   LDA   #$01
.cmp_end@31:

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
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   CLC
   LDA   FP
   ADC   #$17
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
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$2A
   DEY
   STA   (SSP),Y
   LDA   #$B2
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$04
   INY
   STA   (SSP),Y
   JSR   f0
   STA   $86
   LDA   g0
   STA   $85
   LDA   g0+1
   STA   $84
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   LDA   g1+2
   STA   $81
   LDA   g1+3
   STA   $80
   LDA   $85
   EOR   $83
   STA   $83
   LDA   $84
   EOR   $82
   STA   $82
   LDA   #$00
   EOR   $81
   STA   $81
   LDA   #$00
   EOR   $80
   STA   $80
   LDA   $86
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

g1:
   DC.B  $93
   DS.B  5

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$2A
   STA   g0
   RTS
