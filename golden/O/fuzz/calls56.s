SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_f2__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__local_f2__1	EQU	$81
__zpabi_f0__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f0__p1_0	EQU	$84
__zpabi_f0__p1_1	EQU	$85
__zpabi_f0__pp_0	EQU	$86
__zpabi_f0__pp_1	EQU	$87
__local_main__0	EQU	$88
__local_main__1	EQU	$89
__local_main__2	EQU	$8A
__local_main__3	EQU	$8B
__local_main__4	EQU	$8C
__local_main__mbox	EQU	$8D
__local_f0__0	EQU	$91
__local_f0__1	EQU	$92
__local_f0__2	EQU	$93
__local_f0__3	EQU	$94
__local_f0__4	EQU	$95
__local_f0__5	EQU	$96
__local_f0__6	EQU	$97
__local_f0__7	EQU	$98
__local_f0__8	EQU	$99
__local_f0__9	EQU	$9A
__local_f0__10	EQU	$9B
__local_f0__11	EQU	$9C

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__8,__local_f0__9,__local_f0__10,__local_f0__11 indirect=false in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals=__local_f2__0,__local_f2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__mbox,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f1 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item f0@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item g1 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=25
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

   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   ORA   g1+4
   ORA   g1+5
   BEQ   .and_false@1
   INY
   LDA   (FP),Y
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f2__0
   LDY   #$03
   LDA   (FP),Y
   CLC
   ADC   (FP),Y
   STA   __local_f2__1
   LDA   __local_f2__0
   ADC   __local_f2__0
   STA   __local_f2__0
   ORA   __local_f2__1
   BNE   .and_end@2
.and_false@1:
.and_end@2:
   LDY   #$03
   LDA   (FP),Y
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   TAX
   LDY   #$03
   LDA   (FP),Y

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

f1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 1 local bytes, 1 callee-saved bytes
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
   EOR   g1
   STA   $8B
   LDA   g1+1
   EOR   g1+1
   STA   $8A
   LDA   g1+2
   EOR   g1+2
   STA   $89
   LDA   g1+3
   EOR   g1+3
   STA   $88
   LDA   g1+4
   EOR   g1+4
   STA   $87
   LDA   g1+5
   EOR   g1+5
   STA   $86
   LDA   g0
   STA   $85
   LDA   g0+1
   STA   $84
   LDA   g0+2
   STA   $83
   LDA   g0+3
   STA   $82
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   $81
   STA   $80
   LDA   g1
   SEC
   SBC   $85
   LDA   g1+1
   SBC   $84
   LDA   g1+2
   SBC   $83
   LDA   g1+3
   SBC   $82
   LDA   g1+4
   SBC   $81
   LDA   g1+5
   SBC   $80
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   ORA   #$01
   STA   $82
   LDA   #$00
   STA   $81
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   STA   $80
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
   LDA   $86
   STA   HARGS+5
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   LDA   $82
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDA   $80
   STA   HARGS+13
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   STA   $C0
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $89
   LDA   #<f2
   STA   $87
   LDA   #>f2
   STA   $88
   LDA   g0
   EOR   #$FF
   STA   $86
   LDA   g0
   STA   $85
   LDA   g0+1
   STA   $84
   LDA   g0+2
   STA   $83
   LDA   g0+3
   STA   $82
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   $89
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
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $87
   STA   DPTR
   LDA   $88
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $81
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
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
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$03
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

f0:
   SUBROUTINE

   LDA   #$FF
   EOR   g1
   STA   __local_f0__6
   LDA   #$7F
   EOR   g1+1
   STA   __local_f0__5
   LDA   #$00
   EOR   g1+2
   STA   __local_f0__4
   LDA   #$00
   EOR   g1+3
   STA   __local_f0__3
   LDA   #$00
   EOR   g1+4
   STA   __local_f0__2
   LDA   #$00
   EOR   g1+5
   STA   __local_f0__1
   LDA   #$64
   SEC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   LDA   #$00
   SBC   g1+2
   LDA   #$00
   SBC   g1+3
   LDA   #$00
   SBC   g1+4
   LDA   #$00
   SBC   g1+5
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   AND   #$07
   STA   HARGS+8
   LDA   __local_f0__6
   STA   HARGS
   LDA   __local_f0__5
   STA   HARGS+1
   LDA   __local_f0__4
   STA   HARGS+2
   LDA   __local_f0__3
   STA   HARGS+3
   LDA   __local_f0__2
   STA   HARGS+4
   LDA   __local_f0__1
   JSR   f0@run0
   JSR   asr64
   LDA   HARGS+9
   STA   __local_f0__11
   LDA   HARGS+10
   STA   __local_f0__10
   LDA   HARGS+11
   STA   __local_f0__9
   LDA   HARGS+12
   STA   __local_f0__2
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__0
   INY
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__1
   INC   __local_f0__0
   BNE   .inc_done@1
   INC   __local_f0__1
.inc_done@1:
   LDA   __local_f0__0
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__1
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDA   #$FF
   SEC
   SBC   g1
   STA   __local_f0__8
   LDA   #$00
   SBC   g1+1
   STA   __local_f0__7
   LDA   #$00
   SBC   g1+2
   STA   __local_f0__6
   LDA   #$00
   SBC   g1+3
   STA   __local_f0__5
   LDA   #$00
   SBC   g1+4
   STA   __local_f0__4
   LDA   #$00
   SBC   g1+5
   STA   __local_f0__3
   LDA   __local_f0__11
   ORA   __local_f0__10
   ORA   __local_f0__9
   ORA   __local_f0__2
   BEQ   .lnot_true@17
   LDA   #$00
   BEQ   .lnot_end@18
.lnot_true@17:
   LDA   #$01
.lnot_end@18:
   ORA   #$01
   STA   __local_f0__2
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   STA   __local_f0__0
   LDA   __local_f0__8
   STA   HARGS
   LDA   __local_f0__7
   STA   HARGS+1
   LDA   __local_f0__6
   STA   HARGS+2
   LDA   __local_f0__5
   STA   HARGS+3
   LDA   __local_f0__4
   STA   HARGS+4
   LDA   __local_f0__3
   JSR   f0@run0
   LDA   __local_f0__2
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDA   __local_f0__0
   STA   HARGS+13
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+16
   RTS

f0@run0:
   SUBROUTINE

   STA   HARGS+5
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDY   #$82
   STY   __zpabi_f0__p0_0
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   STA   __zpabi_f0__p1_1
   LDA   #$1A
   STA   __zpabi_f0__p1_0
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   JSR   f0
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   g0+2
   EOR   g1+2
   STA   __local_main__1
   LDA   g0+3
   EOR   g1+3
   STA   __local_main__0
   LDA   __local_main__4
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
   DC.L  $000000C4

g1:
   DC.B  $B5
   DS.B  5
