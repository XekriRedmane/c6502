SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f3__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__local_f3__1	EQU	$81
__zpabi_f0__p0_1	EQU	$81
__local_f3__2	EQU	$82
__zpabi_f0__p0_2	EQU	$82
__local_f3__3	EQU	$83
__zpabi_f0__p0_3	EQU	$83
__zpabi_f0__pp_0	EQU	$84
__zpabi_f0__pp_1	EQU	$85
__local_main__0	EQU	$86
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__4	EQU	$8A
__local_main__5	EQU	$8B
__local_main__6	EQU	$8C
__local_main__7	EQU	$8D
__local_main__mbox	EQU	$8E
__local_f0__0	EQU	$92
__local_f0__1	EQU	$93
__local_f0__2	EQU	$94
__local_f0__3	EQU	$95
__local_f0__4	EQU	$96
__local_f0__5	EQU	$97
__local_f0__6	EQU	$98
__local_f0__7	EQU	$99
__local_f0__8	EQU	$9A

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__8 indirect=false in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false carry=true
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__mbox,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; call f1 -> f2
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$28 weight=21
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f3:
   SUBROUTINE

   ; prologue: 3 arg bytes, 0 local bytes
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

   LDA   #$00
   AND   g0
   STA   __local_f3__1
   LDY   #$04
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+6
   ORA   #$01
   STA   HARGS+1
   LDA   __local_f3__1
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+2
   STA   __local_f3__3
   LDA   #$00
   EOR   #$01
   SEC
   SBC   #$01
   STA   __local_f3__2
   LDA   g0
   STA   __local_f3__1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f3__0
   LDA   __local_f3__2
   CMP   __local_f3__0
   BNE   .cmp_true@2
   LDA   __local_f3__3
   CMP   __local_f3__1
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   STA   __local_f3__0
   LDA   __local_f3__3
   SEC
   SBC   __local_f3__0
   LDA   __local_f3__2
   SBC   #$00
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDX   #$00

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$05
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

   ; prologue: 4 arg bytes, 8 local bytes, 8 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
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

   LDA   #<f3
   STA   $C6
   LDA   #>f3
   STA   $C7
   LDY   #$0B
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
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
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BPL   .loop@0_break
   LDA   g0
   STA   $80
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $81
   LDA   $80
   EOR   #$FF
   AND   #$7F
   EOR   #$40
   SEC
   SBC   #$40
   STA   $80
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDA   $C6
   STA   DPTR
   LDA   $C7
   STA   DPTR+1
   JSR   icall
   CLC
   ADC   $C1
   STA   $81
   TXA
   ADC   $C0
   STA   $80
   LDA   $C5
   CLC
   ADC   $81
   STA   $C5
   LDA   $C4
   ADC   $80
   STA   $C4
   BCC   .carry_inc@0
   INC   $C3
   BNE   .carry_inc@0
   INC   $C2
.carry_inc@0:
   INC   $C1
   BNE   .loop@0_start
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$FF
   SEC
   SBC   $C5
   LDA   #$00
   SBC   $C4
   LDA   #$00
   SBC   $C3
   LDA   #$00
   SBC   $C2
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   STA   $80
   LDA   #$00
   BNE   .thread@0
   LDA   $80
   CMP   #$07
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
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

f1:
   SUBROUTINE

   ; prologue: 9 arg bytes, 13 local bytes, 13 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0E
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

   LDY   #$18
   LDA   (FP),Y
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $86
   STA   $85
   STA   $84
   LDA   g0
   STA   $83
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $82
   STA   $81
   STA   $80
   LDA   $83
   CLC
   LDY   #$18
   SBC   (FP),Y
   LDA   $82
   SBC   $86
   LDA   $81
   SBC   $85
   LDA   $80
   SBC   $84
   LDA   #$00
   ROL
   STA   $C0
   LDA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   STA   $CC
   LDA   #$00
   STA   $CB
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   $CC
   STA   $CB
.and_end@1:
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   $CC
   STA   $83
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $82
   INY
   LDA   (FP),Y
   ADC   #$00
   STA   $81
   INY
   LDA   (FP),Y
   ADC   #$00
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
   JSR   f2
   LDA   #$00
   ROL
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $CA
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   $C9
   LDA   g0
   CLC
   ADC   $CA
   STA   g0
   LDA   #<f3
   STA   $C7
   LDA   #>f3
   STA   $C8
   LDY   #$18
   LDA   (FP),Y
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   $C6
   LDA   $CC
   STA   $C5
   LDA   $CB
   STA   $C4
   LDA   #$00
   STA   $C3
   STA   $C2
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@1_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@21
   EOR   #$80
.jcmp_novf@21:
   BMI   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDY   #$18
   LDA   (FP),Y
   SEC
   SBC   $CA
   LDA   $C6
   SBC   $C9
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   AND   #$7F
   EOR   #$40
   SEC
   SBC   #$40
   STA   $81
   LDA   $CB
   CMP   $C9
   BNE   .thread@1
   LDA   $CC
   CMP   $CA
   BEQ   .cmp_true@23
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   STA   $80
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $81
   LDY   #$01
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $C7
   STA   DPTR
   LDA   $C8
   STA   DPTR+1
   JSR   icall
   CLC
   ADC   $C1
   STA   $81
   TXA
   ADC   $C0
   STA   $80
   LDA   $C5
   CLC
   ADC   $81
   STA   $C5
   LDA   $C4
   ADC   $80
   STA   $C4
   BCC   .carry_inc@1
   INC   $C3
   BNE   .carry_inc@1
   INC   $C2
.carry_inc@1:
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@1_start
.loop@1_break:
   LDA   g0
   STA   $80
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   HARGS+1
   LDY   #$18
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   LDA   $C6
   STA   HARGS+3
   LDA   $80
   STA   HARGS
   JSR   sdivmod16
   LDA   HARGS+6
   STA   $C1
   LDA   HARGS+7
   STA   $C0
   LDA   g0
   STA   $80
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   HARGS+1
   LDA   $80
   STA   HARGS
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   HARGS+2
   LDA   HARGS+5
   STA   HARGS+3
   LDA   $C1
   STA   HARGS
   LDA   $C0
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   HARGS+10
   STA   HARGS+11
   LDA   $81
   STA   HARGS+8
   LDA   $80
   STA   HARGS+9

   ; epilogue
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
   CLC
   LDA   FP
   ADC   #$18
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

   LDA   __zpabi_f0__p0_0
   CLC
   SBC   __zpabi_f0__p0_0
   LDA   __zpabi_f0__p0_1
   SBC   __zpabi_f0__p0_1
   LDA   __zpabi_f0__p0_2
   SBC   __zpabi_f0__p0_2
   LDA   __zpabi_f0__p0_3
   SBC   __zpabi_f0__p0_3
   LDA   #$00
   ROL
   AND   #$7F
   EOR   #$40
   SEC
   SBC   #$40
   STA   __local_f0__8
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   __local_f0__7
   LDA   g0
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_f0__1
   LDA   g0
   CLC
   ADC   #$03
   STA   __local_f0__0
   BCC   .carry_inc@2
   INC   __local_f0__1
.carry_inc@2:
   LDA   #$00
   CMP   __local_f0__1
   BNE   .cmp_end@42
   CMP   __local_f0__0
   BNE   .cmp_end@42
   LDA   #$01
.cmp_end@42:
   STA   __local_f0__0
   LDA   #$07
   SEC
   SBC   __local_f0__0
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f0__2
   LDA   #$00
   BPL   .sx_done@45
   LDA   #$FF
.sx_done@45:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __zpabi_f0__p0_3
   CMP   __local_f0__0
   BNE   .thread@4
   LDA   __zpabi_f0__p0_2
   CMP   __local_f0__1
   BNE   .thread@4
   LDA   __zpabi_f0__p0_1
   BNE   .thread@4
   LDA   __zpabi_f0__p0_0
   CMP   __local_f0__2
   BEQ   .cmp_true@46
.thread@4:
   LDA   #$00
   BEQ   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   STA   __local_f0__6
   LDA   #$00
   BPL   .sx_done@50
   LDA   #$FF
.sx_done@50:
   STA   __local_f0__5
   STA   __local_f0__4
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__3
   INY
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__2
   LDA   g0
   BMI   .sx_neg@51
   LDA   #$00
   BEQ   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   CMP   __local_f0__7
   BNE   .thread@5
   LDA   g0
   CMP   __local_f0__8
   BEQ   .cmp_true@53
.thread@5:
   LDA   #$00
   BEQ   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   CLC
   ADC   __local_f0__3
   BCC   .carry_inc@3
   INC   __local_f0__2
.carry_inc@3:
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__2
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDA   g0
   BEQ   .and_false@2
   LDA   __local_f0__6
   ORA   __local_f0__5
   ORA   __local_f0__4
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f0__3
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f0__3
.and_end@3:
   LDA   __local_f0__8
   EOR   #$FF
   STA   __local_f0__1
   LDA   __local_f0__7
   EOR   #$FF
   STA   __local_f0__0
   LDA   #$00
   CMP   __local_f0__0
   BNE   .vthread@2
   LDA   __local_f0__3
   CMP   __local_f0__1
   BEQ   .cmp_true@56
   LDA   #$00
.vthread@2:
   RTS
.cmp_true@56:
   LDA   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$82
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   JSR   f0
   STA   __local_main__7
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   BEQ   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   STA   __local_main__6
   STA   __local_main__5
   STA   __local_main__4
   LDA   g0
   BMI   .sx_neg@61
   LDA   #$00
   BEQ   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__7
   EOR   g0
   STA   __local_main__3
   LDA   __local_main__6
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__5
   EOR   __local_main__1
   STA   HARGS+2
   LDA   __local_main__4
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

   LDA   #$28
   STA   g0
   RTS
