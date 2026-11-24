SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__local_f5__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__local_f5__1	EQU	$81
__zpabi_f0__p0_1	EQU	$81
__local_f5__2	EQU	$82
__zpabi_f0__p0_2	EQU	$82
__local_f5__3	EQU	$83
__zpabi_f0__p0_3	EQU	$83
__local_f5__4	EQU	$84
__zpabi_f3__p0_0	EQU	$84
__local_f5__5	EQU	$85
__zpabi_f3__p0_1	EQU	$85
__local_f5__6	EQU	$86
__zpabi_f3__p0_2	EQU	$86
__local_f5__7	EQU	$87
__zpabi_f3__p0_3	EQU	$87
__local_f5__8	EQU	$88
__local_main__0	EQU	$88
__local_main__1	EQU	$89
__local_main__2	EQU	$8A
__local_main__3	EQU	$8B
__local_f0__l0	EQU	$96
__local_f0__0	EQU	$97
__local_f0__1	EQU	$98
__local_f0__2	EQU	$99
__local_f0__4	EQU	$9B

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3 locals=__local_f0__l0,__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7 indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1,__zpabi_f3__p2 locals=__local_f3__p1 indirect=false in_cycle=false param_regs=,,,,A,X carry=true
; def f4 params= locals= indirect=true in_cycle=false carry=true
; def f5 params= locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3,__local_f5__4,__local_f5__5,__local_f5__6,__local_f5__7,__local_f5__8 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f0 -> f3
; call f1 -> f2
; call f2 -> f3
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$5D weight=12
; zp g0 size=4 at=auto def=true init=$1A,$00,$00,$00 weight=13
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f5:
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

   INY
   LDA   (FP),Y
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f5__8
   LDA   g1
   STA   __local_f5__1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f5__0
   LDY   #$03
   LDA   (FP),Y
   EOR   __local_f5__1
   STA   __local_f5__7
   LDA   __local_f5__8
   EOR   __local_f5__0
   STA   __local_f5__6
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f5__5
   STA   __local_f5__4
   LDA   g0
   STA   __local_f5__3
   LDA   g0+1
   STA   __local_f5__2
   LDA   g0+2
   STA   __local_f5__1
   LDA   g0
   SEC
   SBC   __local_f5__7
   LDA   g0+1
   SBC   __local_f5__6
   LDA   g0+2
   SBC   __local_f5__5
   LDA   g0+3
   SBC   __local_f5__4
   LDA   #$00
   ROL
   STA   __local_f5__0
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f5__3
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_f5__2
   LDY   #$03
   LDA   (FP),Y
   ORA   __local_f5__0
   STA   __local_f5__1
   LDA   __local_f5__8
   STA   __local_f5__0
   LDA   __local_f5__3
   CLC
   ADC   __local_f5__1
   STA   __local_f5__1
   LDA   __local_f5__2
   ADC   __local_f5__0
   STA   __local_f5__0
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f5__1
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f5__0
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

f4:
   SUBROUTINE

   ; prologue: 15 arg bytes, 5 local bytes, 3 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
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

   LDA   g1
   STA   $82
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   $83
   LDY   #$14
   LDA   (FP),Y
   AND   #$07
   TAX
   BEQ   .shift_done@9
.shift_loop@8:
   ASL   $82
   ROL   $83
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   $83
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $81
   STA   $80
   LDY   #$08
   LDA   (FP),Y
   SEC
   SBC   $82
   INY
   LDA   (FP),Y
   SBC   $83
   INY
   LDA   (FP),Y
   SBC   $81
   INY
   LDA   (FP),Y
   SBC   $80
   LDA   #$00
   ROL
   STA   $C2
   LDY   #$15
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $C1
   LDY   #$15
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $C0
   LDA   g1
   STA   $81
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   $80
   LDA   $81
   ORA   #$01
   STA   $81
   LDA   $80
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$0C
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
   LDA   $81
   STA   HARGS+4
   LDA   $80
   STA   HARGS+5
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   STA   $80
   LDA   $C1
   CLC
   ADC   $81
   STA   $81
   LDA   $C0
   ADC   $80
   STA   $80
   LDY   #$15
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   $C2
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<f5
   STA   $85
   LDA   #>f5
   STA   $86
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   ORA   (FP),Y
   STA   $84
   LDY   #$0D
   LDA   (FP),Y
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   $83
   STA   $82
   CLC
   LDA   FP
   ADC   #$04
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $84
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$03
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
   LDY   #$04
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   LDA   $C2
   EOR   $80
   CLC
   ADC   $81
   CLC
   ADC   g1
   STA   g1
   LDY   #$0C
   LDA   (FP),Y
   SEC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   INY
   LDA   (FP),Y
   SBC   g0+2
   INY
   LDA   (FP),Y
   SBC   g0+3
   BCS   .and_false@0
   LDA   #$01
   BNE   .and_end@1
.and_false@0:
   LDA   #$00
.and_end@1:
   CMP   #$01

   ; epilogue
   PHP
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
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLP
   RTS

f3:
   SUBROUTINE

   CLC
   RTS

f2:
   SUBROUTINE

   ; prologue: 6 arg bytes, 6 local bytes, 4 callee-saved bytes
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

   LDY   #$09
   LDA   (FP),Y
   STA   $80
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   CPX   #$00
   BEQ   .shift_done@21
.shift_loop@20:
   LSR
   DEX
   BNE   .shift_loop@20
.shift_done@21:
   STA   $C1
   STA   $C0
   LDX   g0
   LDA   #$01
   STA   __zpabi_f3__p0_0
   LDA   #$00
   STA   __zpabi_f3__p0_1
   STA   __zpabi_f3__p0_2
   STA   __zpabi_f3__p0_3
   LDA   $C1
   JSR   f3
   BCC   .if_else@7
   LDA   $C0
   CLC
   ADC   #$03
   STA   $C3
   LDA   #$00
   ADC   #$00
   STA   $C2
   LDA   #$00
   ADC   #$00
   STA   $C1
   LDA   #$00
   ADC   #$00
   STA   $C0
   JMP   .if_end@6
.if_else@7:
   LDA   $C1
   EOR   #$05
   STA   $C3
   LDA   #$00
   STA   $C2
   STA   $C1
   STA   $C0
.if_end@6:
   LDA   $C3
   LDY   #$05
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   #<f5
   STA   $88
   LDA   #>f5
   STA   $89
   LDY   #$0E
   LDA   (FP),Y
   SEC
   SBC   g1
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
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $82
   LDY   #$01
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
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
   LDA   $88
   STA   DPTR
   LDA   $89
   STA   DPTR+1
   JSR   icall
   STA   $83
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   $82
   STA   $81
   STA   $80
   LDA   $C3
   EOR   $83
   STA   $8B
   LDA   $C2
   EOR   $82
   STA   $8A
   LDA   $C1
   EOR   $81
   STA   $89
   LDA   $C0
   EOR   $80
   STA   $88
   LDY   #$05
   LDA   (FP),Y
   STA   $83
   INY
   LDA   (FP),Y
   STA   $82
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $81
   STA   $80
   LDA   $8B
   CLC
   ADC   $83
   STA   HARGS+4
   LDA   $8A
   ADC   $82
   STA   HARGS+5
   LDA   $89
   ADC   $81
   STA   HARGS+6
   LDA   $88
   ADC   $80
   STA   HARGS+7
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   JSR   mul32
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   LDA   HARGS+10
   STA   $81
   LDA   HARGS+11
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   $83
   STA   $83
   INY
   LDA   (FP),Y
   ADC   $82
   STA   $82
   INY
   LDA   (FP),Y
   ADC   $81
   STA   $81
   INY
   LDA   (FP),Y
   ADC   $80
   ORA   $81
   ORA   $82
   ORA   $83
   BEQ   .cmp_end@26
   LDA   #$01
.cmp_end@26:
   CMP   #$01

   ; epilogue
   PHP
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
   ADC   #$0E
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
   PLP
   RTS

f1:
   SUBROUTINE

   ; prologue: 8 arg bytes, 0 local bytes
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
   STA   $83
   LDA   g0+1
   STA   $82
   LDA   g0+2
   STA   $81
   LDA   g0+3
   STA   $80
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDA   $83
   INY
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
   LDY   #$07
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   f2
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   INY
   LDA   (FP),Y
   SBC   g0+2
   INY
   LDA   (FP),Y
   SBC   g0+3
   LDA   #$00
   ROL
   STA   $81
   LDA   #$00
   CMP   g0+3
   BNE   .cmp_end@28
   CMP   g0+2
   BNE   .cmp_end@28
   CMP   g0+1
   BNE   .cmp_end@28
   LDA   #$03
   CMP   g0
   BEQ   .cmp_true@27
   LDA   #$00
   BEQ   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   AND   $81
   LDX   #$00

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0A
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

f0:
   SUBROUTINE

   LDA   g1
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   __local_f0__l0
   LDA   __zpabi_f0__p0_0
   CLC
   ADC   g1
   STA   __local_f0__0
   LDA   __zpabi_f0__p0_1
   ADC   __local_f0__l0
   STA   __local_f0__l0
   LDA   g0
   AND   __local_f0__0
   STA   __local_f0__4
   LDA   g0+1
   AND   __local_f0__l0
   BNE   .cmp_true@32
   LDA   __local_f0__4
   CMP   #$01
   BNE   .cmp_true@32
   LDA   #$00
   BEQ   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   STA   __local_f0__2
   LDA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g0
   ORA   #$01
   STA   HARGS+4
   LDA   g0+1
   STA   HARGS+5
   LDA   g0+2
   STA   HARGS+6
   LDA   g0+3
   STA   HARGS+7
   LDA   __zpabi_f0__p0_0
   STA   HARGS
   LDA   __zpabi_f0__p0_1
   STA   HARGS+1
   LDA   __zpabi_f0__p0_2
   STA   HARGS+2
   LDA   __zpabi_f0__p0_3
   STA   HARGS+3
   JSR   udivmod32
   LDA   #$FF
   SEC
   SBC   __local_f0__4
   TAX
   LDA   __local_f0__2
   STA   __zpabi_f3__p0_0
   LDA   #$00
   STA   __zpabi_f3__p0_1
   LDA   __local_f0__1
   STA   __zpabi_f3__p0_2
   LDA   __local_f0__0
   STA   __zpabi_f3__p0_3
   LDA   HARGS+8
   JSR   f3
   BCC   .if_else@9
   LDA   __local_f0__4
   CLC
   ADC   #$03
   STA   __local_f0__l0
   JMP   .if_end@8
.if_else@9:
   LDA   __local_f0__4
   EOR   #$05
   STA   __local_f0__l0
.if_end@8:
   LDA   g1
   CLC
   ADC   __local_f0__l0
   STA   g1
   EOR   #$FF
   STA   __local_f0__l0
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f0__l0
   STA   HARGS+8
   RTS

main:
   SUBROUTINE

   LDA   #$20
   STA   __zpabi_f0__p0_0
   LDA   #$01
   STA   __zpabi_f0__p0_1
   LDA   #$00
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   JSR   f0
   LDA   g1
   BMI   .sx_neg@41
   LDA   #$00
   BEQ   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   __local_main__2
   STA   __local_main__2
   LDA   g0+2
   EOR   __local_main__1
   STA   __local_main__1
   LDA   g0+3
   EOR   __local_main__0
   STA   __local_main__0
   LDA   HARGS+8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   EOR   __local_main__1
   STA   HARGS+2
   LDA   HARGS+11
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

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   LDA   #$1A
   STA   g0
   LDA   #$5D
   STA   g1
   RTS
