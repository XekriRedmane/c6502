SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f3__0	EQU	$80
__local_f3__1	EQU	$81
__local_f3__2	EQU	$82
__local_f3__3	EQU	$83
__local_f3__4	EQU	$84
__local_f3__5	EQU	$85
__local_f3__6	EQU	$86
__local_f3__7	EQU	$87
__local_f3__8	EQU	$88
__local_f3__9	EQU	$89
__local_f3__10	EQU	$8A
__local_f3__11	EQU	$8B

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__6,__local_f3__7,__local_f3__8,__local_f3__9,__local_f3__10,__local_f3__11 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> f2
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=5 at=auto def=true init=$05,$00,$00,$00,$00 weight=17
; runtime hargs_size=32 hargs_need=21
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

f3:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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
   STA   __local_f3__11
   LDA   g0+1
   STA   __local_f3__10
   ORA   g0
   BNE   .or_true@1
   LDA   __local_f3__10
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f3__2
   STA   __local_f3__1
   LDA   __local_f3__11
   STA   __local_f3__4
   LDA   __local_f3__10
   STA   __local_f3__3
   LDA   __local_f3__2
   AND   #$3F
   STA   __local_f3__0
   CMP   g0+4
   BNE   .or_true@1
   LDA   __local_f3__1
   CMP   g0+3
   BNE   .or_true@1
   CMP   g0+2
   BNE   .or_true@1
   LDA   __local_f3__3
   CMP   g0+1
   BNE   .or_true@1
   LDA   __local_f3__4
   CMP   g0
   BNE   .or_true@1
   LDA   #$00
   STA   __local_f3__4
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   __local_f3__4
.or_end@2:
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f3__6
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_f3__5
   LDA   __local_f3__4
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f3__3
   STA   __local_f3__2
   STA   __local_f3__1
   AND   #$3F
   STA   __local_f3__0
   LDA   __local_f3__4
   CLC
   SBC   g0
   LDA   __local_f3__3
   SBC   g0+1
   LDA   __local_f3__2
   SBC   g0+2
   LDA   __local_f3__1
   SBC   g0+3
   LDA   __local_f3__0
   SBC   g0+4
   LDA   #$00
   ROL
   CLC
   ADC   __local_f3__6
   STA   __local_f3__0
   BCC   .carry_inc@0
   INC   __local_f3__5
.carry_inc@0:
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f3__0
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f3__5
   LDY   #$01
   STA   (DPTR),Y
   LDA   g0
   EOR   g0
   STA   __local_f3__9
   LDA   g0+1
   EOR   g0+1
   STA   __local_f3__8
   LDA   g0+2
   EOR   g0+2
   STA   __local_f3__7
   LDA   g0+3
   EOR   g0+3
   STA   __local_f3__6
   LDA   g0+4
   EOR   g0+4
   STA   __local_f3__5
   LDA   __local_f3__10
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f3__2
   STA   __local_f3__1
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDX   __local_f3__11
   STX   __local_f3__4
   LDX   __local_f3__10
   STX   __local_f3__3
   AND   #$3F
   STA   __local_f3__0
   LDA   __local_f3__4
   CLC
   SBC   __local_f3__9
   LDA   __local_f3__3
   SBC   __local_f3__8
   LDA   __local_f3__2
   SBC   __local_f3__7
   LDA   __local_f3__1
   SBC   __local_f3__6
   LDA   __local_f3__0
   SBC   __local_f3__5
   LDA   #$00
   ROL
   STA   __local_f3__0
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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

f2:
   SUBROUTINE

   ; prologue: 7 arg bytes, 10 local bytes, 8 callee-saved bytes
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

   LDY   #$0D
   LDA   (FP),Y
   EOR   #$FF
   STA   $C7
   LDA   #$00
   EOR   #$FF
   STA   $C6
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   $82
   LDA   $C7
   EOR   #$FF
   STA   $81
   LDA   $C6
   EOR   #$FF
   STA   $C5
   LDY   #$0D
   LDA   (FP),Y
   STA   $80
   LDA   $81
   SEC
   SBC   $80
   STA   $C4
   BCS   .carry_inc@1
   DEC   $C5
.carry_inc@1:
   LDY   #$0F
   LDA   (FP),Y
   ORA   #$01
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
   LDA   $C4
   STA   HARGS
   LDA   $C5
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+12
   STA   $C3
   LDA   HARGS+13
   STA   $C2
   LDA   HARGS+14
   STA   $C1
   LDA   HARGS+15
   STA   $C0
   LDA   $C7
   LDY   #$09
   STA   (FP),Y
   LDA   $C6
   INY
   STA   (FP),Y
   LDA   #<f3
   STA   $82
   LDA   #>f3
   STA   $83
   CLC
   LDA   FP
   ADC   #$09
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $82
   STA   DPTR
   LDA   $83
   STA   DPTR+1
   JSR   icall
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   STA   $80
   LDA   $C4
   EOR   $81
   STA   $81
   LDA   $C5
   EOR   $80
   STA   $80
   LDA   $81
   CLC
   LDY   #$09
   ADC   (FP),Y
   STA   $84
   LDA   $80
   INY
   ADC   (FP),Y
   STA   $83
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $82
   STA   $81
   AND   #$3F
   STA   $80
   LDA   g0
   CLC
   ADC   $84
   STA   $84
   LDA   g0+1
   ADC   $83
   STA   $83
   LDA   g0+2
   ADC   $82
   STA   $82
   LDA   g0+3
   ADC   $81
   STA   $81
   LDA   g0+4
   ADC   $80
   STA   $80
   LDA   $84
   STA   g0
   LDA   $83
   STA   g0+1
   LDA   $82
   STA   g0+2
   LDA   $81
   STA   g0+3
   LDA   $80
   AND   #$3F
   STA   g0+4
   LDY   #$13
   LDA   (FP),Y
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $83
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   $82
   STA   $81
   LDY   #$13
   LDA   (FP),Y
   STA   $84
   LDA   $82
   AND   #$3F
   STA   $80
   LDA   g0
   AND   $84
   STA   $88
   LDA   g0+1
   AND   $83
   STA   $87
   LDA   g0+2
   AND   $82
   STA   $86
   LDA   g0+3
   AND   $81
   STA   $85
   LDA   g0+4
   AND   $80
   STA   $84
   LDY   #$0F
   LDA   (FP),Y
   CLC
   ADC   $C3
   STA   $83
   INY
   LDA   (FP),Y
   ADC   $C2
   STA   $82
   INY
   LDA   (FP),Y
   ADC   $C1
   STA   $81
   INY
   LDA   (FP),Y
   ADC   $C0
   STA   $80
   LDA   $84
   BNE   .thread@0
   LDA   $85
   CMP   $80
   BNE   .thread@0
   LDA   $86
   CMP   $81
   BNE   .thread@0
   LDA   $87
   CMP   $82
   BNE   .thread@0
   LDA   $88
   CMP   $83
   BEQ   .cmp_true@19
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
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
   CLC
   LDA   FP
   ADC   #$13
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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

   ; prologue: 4 arg bytes, 3 local bytes, 1 callee-saved bytes
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

   LDA   g0
   STA   $8B
   LDA   g0+1
   STA   $8A
   LDY   #$06
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   STA   $84
   LDA   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   STA   $84
.and_end@4:
   ORA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   $82
   LDA   $84
   STA   $89
   LDA   #$00
   STA   $88
   LDA   $82
   STA   $87
   STA   $86
   AND   #$3F
   STA   $85
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   g0
   STA   $84
   INY
   LDA   (FP),Y
   ADC   g0+1
   STA   $83
   INY
   LDA   (FP),Y
   ADC   g0+2
   STA   $82
   INY
   LDA   (FP),Y
   ADC   g0+3
   STA   $81
   LDA   #$00
   ADC   g0+4
   AND   #$3F
   STA   $80
   LDA   $85
   CMP   $80
   BNE   .cmp_true@24
   LDA   $86
   CMP   $81
   BNE   .cmp_true@24
   CMP   $82
   BNE   .cmp_true@24
   LDA   #$00
   CMP   $83
   BNE   .cmp_true@24
   LDA   $89
   CMP   $84
   BNE   .cmp_true@24
   LDA   #$00
   BEQ   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   STA   $C0
   LDA   $8B
   LDY   #$02
   STA   (FP),Y
   LDA   $8A
   INY
   STA   (FP),Y
   LDA   #<f3
   STA   $82
   LDA   #>f3
   STA   $83
   CLC
   LDA   FP
   ADC   #$02
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $82
   STA   DPTR
   LDA   $83
   STA   DPTR+1
   JSR   icall
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   STA   $80
   LDA   #$00
   EOR   $81
   STA   $83
   LDA   #$FF
   EOR   $80
   STA   $82
   LDY   #$02
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   STA   $80
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   $80
   STA   $80
   LDA   g0
   CLC
   ADC   $81
   STA   $84
   LDA   g0+1
   ADC   $80
   STA   $83
   LDA   g0+2
   ADC   #$00
   STA   $82
   LDA   g0+3
   ADC   #$00
   STA   $81
   LDA   g0+4
   ADC   #$00
   STA   $80
   LDA   $84
   STA   g0
   LDA   $83
   STA   g0+1
   LDA   $82
   STA   g0+2
   LDA   $81
   STA   g0+3
   LDA   $80
   AND   #$3F
   STA   g0+4
   LDA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   STA   $81
   STA   $80
   LDA   $C0
   LDY   #$06
   ORA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   LDA   $81
   INY
   ORA   (FP),Y
   STA   HARGS+10
   LDA   $80
   INY
   ORA   (FP),Y
   STA   HARGS+11

   ; epilogue
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$09
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
   RTS

f0:
   SUBROUTINE

   ; prologue: 12 arg bytes, 8 local bytes, 8 callee-saved bytes
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

   LDY   #$0B
   LDA   (FP),Y
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   $C7
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   $82
   STA   $81
   STA   $80
   LDY   #$0B
   LDA   (FP),Y
   STA   HARGS
   LDA   $C7
   STA   HARGS+1
   LDA   $82
   STA   HARGS+2
   STA   HARGS+3
   AND   #$3F
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   INY
   LDA   (FP),Y
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
   LDA   #$00
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   STA   $84
   LDA   HARGS+17
   STA   $83
   LDA   HARGS+18
   STA   $82
   LDA   HARGS+19
   STA   $81
   LDA   HARGS+20
   AND   #$3F
   STA   $80
   LDA   $84
   ORA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BEQ   .and_false@5
   LDA   #$01
   STA   $C6
   LDA   #$00
   STA   $C5
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   STA   $C6
   STA   $C5
.and_end@6:
   LDY   #$0B
   LDA   (FP),Y
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   $83
   STA   $82
   STA   $81
   LDY   #$0B
   LDA   (FP),Y
   STA   $84
   LDA   $83
   AND   #$3F
   STA   $80
   LDA   $84
   ORA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BEQ   .and_false@9
   LDA   #$01
   STA   $84
   LDA   #$00
   STA   $83
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   STA   $84
   LDY   #$00
   STY   $83
.and_end@10:
   ORA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   $82
   STA   $81
   STA   $80
   LDA   $84
   STA   $C4
   LDA   #$00
   STA   $C3
   LDA   $82
   STA   $C2
   STA   $C1
   AND   #$3F
   STA   $C0
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
   INY
   LDA   (FP),Y
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$00
   STY   HARGS+9
   STA   HARGS+10
   LDA   $C6
   STA   HARGS+8
   STY   HARGS+11
   STY   HARGS+12
   TYA
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   $C4
   CLC
   ADC   HARGS+16
   AND   #$1F
   EOR   #$10
   SEC
   SBC   #$10
   STA   $84
   LDA   #$00
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   LDA   #$00
   SBC   g0+2
   LDA   #$00
   SBC   g0+3
   LDA   #$00
   SBC   g0+4
   LDA   #$00
   ROL
   STA   $83
   LDA   #$02
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDA   #$00
   SBC   $C7
   BVC   .cmp_novf@37
   EOR   #$80
.cmp_novf@37:
   ASL
   LDA   #$00
   ROL
   STA   $82
   LDA   #$00
   BPL   .sx_done@39
   LDA   #$FF
.sx_done@39:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
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
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   JSR   f2
   STA   $81
   STX   $80
   LDA   $C6
   EOR   $81
   STA   $81
   LDA   $C5
   EOR   $80
   STA   $80
   LDA   g0
   CLC
   ADC   $81
   STA   $84
   LDA   g0+1
   ADC   $80
   STA   $83
   LDA   g0+2
   ADC   #$00
   STA   $82
   LDA   g0+3
   ADC   #$00
   STA   $81
   LDA   g0+4
   ADC   #$00
   STA   $80
   LDA   $84
   STA   g0
   LDA   $83
   STA   g0+1
   LDA   $82
   STA   g0+2
   LDA   $81
   STA   g0+3
   LDA   $80
   AND   #$3F
   STA   g0+4
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
   JSR   f1
   LDA   #$FF
   CLC
   LDY   #$0B
   ADC   (FP),Y

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
   CLC
   LDA   FP
   ADC   #$16
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
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$54
   DEY
   STA   (SSP),Y
   LDA   #$0B
   INY
   STA   (SSP),Y
   LDA   #$01
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
   LDA   #$59
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
   JSR   f0
   STA   $87
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   BEQ   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   STA   $86
   STA   $85
   STA   $84
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   LDA   g0+2
   STA   $81
   LDA   g0+3
   STA   $80
   LDA   $87
   EOR   $83
   STA   $C1
   LDA   $86
   EOR   $82
   STA   $C0
   LDA   $85
   EOR   $81
   STA   HARGS+2
   LDA   $84
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   STA   g0+4
   LDA   #$05
   STA   g0
   RTS
