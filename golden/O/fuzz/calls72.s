SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g1	EQU	$28
__local_f2__0	EQU	$80
__local_main__0	EQU	$80
__local_f2__1	EQU	$81
__local_main__1	EQU	$81
__local_f2__2	EQU	$82
__local_main__2	EQU	$82
__local_f2__3	EQU	$83
__local_main__3	EQU	$83
__local_f2__4	EQU	$84
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87
__local_main__8	EQU	$88
__local_f0__0	EQU	$8B
__local_f0__1	EQU	$8C
__local_f0__3	EQU	$8E
__local_f0__4	EQU	$8F
__local_f0__5	EQU	$90
__local_f0__6	EQU	$91
__local_f0__7	EQU	$92

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7 indirect=false in_cycle=false carry=true
; def f1 params= locals= indirect=true in_cycle=false carry=true
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=2 at=auto def=true init=$B3,$00 weight=12
; zp g1 size=4 at=auto def=true init=$4E,$00,$00,$00 weight=15
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

f2:
   SUBROUTINE

   ; prologue: 17 arg bytes, 0 local bytes
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

   LDA   #$64
   ORA   g2
   STA   __local_f2__4
   LDA   g2+1
   STA   __local_f2__3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__1
   STA   __local_f2__0
   LDY   #$07
   LDA   (FP),Y
   AND   #$07
   TAX
   LDA   g0
   STA   __local_f2__2
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   __local_f2__4
   INY
   LDA   (FP),Y
   SBC   __local_f2__3
   INY
   LDA   (FP),Y
   SBC   __local_f2__1
   INY
   LDA   (FP),Y
   SBC   __local_f2__0
   LDA   #$00
   ROL
   STA   __local_f2__1
   CPX   #$00
   BEQ   .shift_done@5
.shift_loop@4:
   ASL   __local_f2__1
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDA   __local_f2__2
   EOR   g1
   ORA   __local_f2__1

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$13
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

   ; prologue: 10 arg bytes, 19 local bytes, 19 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$14
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
   LDA   $D1
   INY
   STA   (FP),Y
   LDA   $D2
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
   STA   $81
   LDA   g2+1
   BNE   .thread@0
   LDA   g2
   LDY   #$1F
   CMP   (FP),Y
   BEQ   .cmp_true@8
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   $80
   LDA   $82
   EOR   $80
   STA   $D2
   LDA   $81
   STA   $D1
   LDY   #$16
   LDA   (FP),Y
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   $D0
   LDY   #$16
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
   STA   $83
   STA   $82
   STA   $81
   STA   $80
   LDY   #$17
   LDA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   SBC   $86
   INY
   LDA   (FP),Y
   SBC   $85
   INY
   LDA   (FP),Y
   SBC   $84
   INY
   LDA   (FP),Y
   SBC   $83
   INY
   LDA   (FP),Y
   SBC   $82
   INY
   LDA   (FP),Y
   SBC   $81
   INY
   LDA   (FP),Y
   SBC   $80
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   STA   $81
   LDY   #$1E
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   AND   $81
   STA   $CF
   LDA   #$00
   STA   $CE
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   $CD
   STA   $CC
   LDA   g0
   CLC
   ADC   g2
   STA   $81
   LDA   g0+1
   ADC   g2+1
   STA   $80
   LDA   $81
   SEC
   SBC   #$03
   LDA   $80
   SBC   #$00
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   STA   $C9
   LDA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   $C7
   STA   $C6
   STA   $C5
   STA   $C4
   STA   $C3
   STA   $C2
   LDA   #<f2
   STA   $CA
   LDA   #>f2
   STA   $CB
   LDA   #$00
   STA   $C8
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@0_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@24
   EOR   #$80
.jcmp_novf@24:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   $D2
   SEC
   LDY   #$16
   SBC   (FP),Y
   LDA   $D1
   SBC   $D0
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   STA   $8A
   LDA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   $89
   STA   $88
   LDA   g2
   STA   $87
   LDA   g1
   STA   $86
   LDA   g1+1
   STA   $85
   LDA   g1+2
   STA   $84
   LDA   g1+3
   STA   $83
   LDY   #$1F
   LDA   (FP),Y
   CLC
   SBC   #$00
   LDA   #$00
   ROL
   STA   $82
   LDA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $8A
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
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
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $CA
   STA   DPTR
   LDA   $CB
   STA   DPTR+1
   JSR   icall
   CLC
   ADC   $C1
   STA   $87
   LDA   #$00
   ADC   $C0
   STA   $86
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   $85
   STA   $84
   STA   $83
   STA   $82
   STA   $81
   STA   $80
   LDA   $C9
   CLC
   ADC   $87
   STA   $87
   LDA   $C8
   ADC   $86
   STA   $86
   LDA   $C7
   ADC   $85
   STA   $85
   LDA   $C6
   ADC   $84
   STA   $84
   LDA   $C5
   ADC   $83
   STA   $83
   LDA   $C4
   ADC   $82
   STA   $82
   LDA   $C3
   ADC   $81
   STA   $81
   LDA   $C2
   ADC   $80
   AND   #$01
   STA   $80
   LDA   $87
   STA   $C9
   LDA   $86
   STA   $C8
   LDA   $85
   STA   $C7
   LDA   $84
   STA   $C6
   LDA   $83
   STA   $C5
   LDA   $82
   STA   $C4
   LDA   $81
   STA   $C3
   LDA   $80
   EOR   #$01
   SEC
   SBC   #$01
   STA   $C2
   INC   $C1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   $CF
   ORA   $CE
   ORA   $CD
   ORA   $CC
   BNE   .or_true@0
   LDY   #$17
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
   BNE   .or_true@0
   LDA   #$00
   STA   $83
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   $83
   LDA   #$00
.or_end@1:
   ORA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
   STA   $81
   STA   $80
   LDA   $CF
   SEC
   SBC   $83
   LDA   $CE
   SBC   #$00
   LDA   $CD
   SBC   $81
   LDA   $CC
   SBC   $80

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
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   CLC
   LDA   FP
   ADC   #$1F
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

f0:
   SUBROUTINE

   LDA   g1
   ASL
   STA   __local_f0__4
   LDA   g1+1
   ROL
   STA   __local_f0__5
   LDA   g1+2
   ROL
   STA   __local_f0__6
   LDA   g1+3
   ROL
   ASL   __local_f0__4
   ROL   __local_f0__5
   ROL   __local_f0__6
   ROL
   STA   __local_f0__7
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f0__3
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f0__3
.and_end@3:
   ORA   #$00
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __local_f0__3
   SEC
   SBC   __local_f0__4
   LDA   #$00
   SBC   __local_f0__5
   LDA   __local_f0__1
   SBC   __local_f0__6
   LDA   __local_f0__0
   SBC   __local_f0__7
   RTS

main:
   SUBROUTINE

   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g0+1
   BMI   .sx_neg@36
   LDA   #$00
   BEQ   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__7
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__6
   LDA   __local_main__5
   EOR   g1+2
   STA   __local_main__5
   LDA   __local_main__4
   EOR   g1+3
   STA   __local_main__4
   LDA   g2+1
   BMI   .sx_neg@38
   LDA   #$00
   BEQ   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__7
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__6
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__1
   LDA   __local_main__4
   EOR   __local_main__0
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

g0:
   DC.W  $0058

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g2+1
   LDA   #$4E
   STA   g1
   LDA   #$B3
   STA   g2
   RTS
