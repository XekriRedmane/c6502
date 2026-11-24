SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p1_0	EQU	$80
__zpabi_f2__p1_0	EQU	$80
__zpabi_f3__p0_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f2__p1_1	EQU	$81
__zpabi_f3__p0_1	EQU	$81
__zpabi_f1__p0_0	EQU	$82
__zpabi_f2__p1_2	EQU	$82
__zpabi_f3__p0_2	EQU	$82
__zpabi_f1__p0_1	EQU	$83
__zpabi_f2__p1_3	EQU	$83
__zpabi_f3__p0_3	EQU	$83
__zpabi_f1__p0_2	EQU	$84
__zpabi_f2__p2_0	EQU	$84
__zpabi_f3__p1_0	EQU	$84
__zpabi_f1__p0_3	EQU	$85
__zpabi_f2__p2_1	EQU	$85
__zpabi_f3__p1_1	EQU	$85
__zpabi_f1__p1_0	EQU	$86
__zpabi_f2__p2_2	EQU	$86
__zpabi_f3__p1_2	EQU	$86
__zpabi_f1__p1_1	EQU	$87
__zpabi_f2__p2_3	EQU	$87
__zpabi_f3__p1_3	EQU	$87
__local_f2__0	EQU	$88
__zpabi_f1__p2_0	EQU	$88
__zpabi_f3__p2_0	EQU	$88
__local_f2__1	EQU	$89
__zpabi_f1__p2_1	EQU	$89
__zpabi_f3__p2_1	EQU	$89
__local_f2__2	EQU	$8A
__zpabi_f1__p2_2	EQU	$8A
__zpabi_f3__p2_2	EQU	$8A
__zpabi_f1__p2_3	EQU	$8B
__zpabi_f3__p2_3	EQU	$8B
__zpabi_f1__p3	EQU	$8C
__zpabi_f3__p3	EQU	$8C
__local_f3__0	EQU	$8D
__local_main__0	EQU	$8D
__local_f3__1	EQU	$8E
__local_main__1	EQU	$8E
__local_f3__2	EQU	$8F
__local_main__2	EQU	$8F
__local_f3__3	EQU	$90
__local_main__3	EQU	$90
__local_main__4	EQU	$91
__local_main__5	EQU	$92
__local_f3__it_1	EQU	$93
__local_main__6	EQU	$93
__local_f3__it_0	EQU	$94
__local_main__8	EQU	$95
__local_f0__0	EQU	$98
__local_f0__1	EQU	$99
__local_f0__p0_0	EQU	$9E
__local_f1__0	EQU	$9F
__local_f4__0	EQU	$9F
__local_f1__1	EQU	$A0
__local_f4__1	EQU	$A0
__local_f1__2	EQU	$A1
__local_f4__2	EQU	$A1
__local_f1__3	EQU	$A2
__local_f4__3	EQU	$A2
__local_f4__it_1	EQU	$A5
__local_f4__it_0	EQU	$A6
__local_f4__l0_3	EQU	$A7
__local_f4__l0_2	EQU	$A8
__local_f4__l0_1	EQU	$A9
__local_f4__l0_0	EQU	$AA
__local_f5__0	EQU	$AB
__local_f5__1	EQU	$AC
__local_f5__2	EQU	$AD
__local_f5__4	EQU	$AF

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__p0_1,__local_f0__p0_0 indirect=false in_cycle=false param_regs=A,X,, carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3,__zpabi_f1__p3 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false param_regs=A,,,,,,,,
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p1_2,__zpabi_f3__p1_3,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p2_2,__zpabi_f3__p2_3,__zpabi_f3__p3 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__it_1,__local_f3__it_0,__local_f3__l1_3,__local_f3__l1_2,__local_f3__l1_1,__local_f3__l1_0 indirect=false in_cycle=false changes=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3 carry=true
; def f4 params= locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__it_1,__local_f4__it_0,__local_f4__l0_3,__local_f4__l0_2,__local_f4__l0_1,__local_f4__l0_0 indirect=false in_cycle=false
; def f5 params= locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3,__local_f5__4 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f4
; call f3 -> f5
; call f4 -> f5
; call main -> f0
; item f5 section=.text local=false zero=false
; item f5@tail0 section=.text local=true zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f3@tail1 section=.text local=true zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$42 weight=12
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f5:
   SUBROUTINE

   LDA   g1
   CLC
   SBC   g1
   LDA   g1+1
   SBC   g1+1
   LDA   g1+2
   SBC   g1+2
   LDA   g1+3
   SBC   g1+3
   LDA   #$00
   ROL
   STA   __local_f5__4
   LDA   g0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f5__2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   CMP   g1+3
   BNE   .cmp_true@4
   CMP   g1+2
   BNE   .cmp_true@4
   LDA   __local_f5__2
   CMP   g1+1
   BNE   .cmp_true@4
   LDA   g0
   CMP   g1
   BNE   .cmp_true@4
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   HARGS+2
   LDA   __local_f5__4
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_f5__1
   STA   __local_f5__0
   LDA   HARGS+4
   ORA   HARGS+5
   ORA   __local_f5__1
   ORA   __local_f5__0
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f5__2
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f5__2
.and_end@1:
   LDA   #$FF
   CMP   HARGS+4
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   SEC
   SBC   __local_f5__2
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   STA   __local_f5__0
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f5__0
   JMP   f5@tail0

f5@tail0:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f4:
   SUBROUTINE

   LDA   g0
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f4__2
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f4__1
   STA   __local_f4__0
   LDA   g0
   SEC
   SBC   g1
   LDA   __local_f4__2
   SBC   g1+1
   LDA   __local_f4__1
   SBC   g1+2
   LDA   __local_f4__0
   SBC   g1+3
   BCC   .or_true@2
   LDA   g0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f4__0
   LDA   #$01
   SBC   g0
   STA   __local_f4__1
   LDA   #$00
   SBC   __local_f4__0
   ORA   __local_f4__1
   BNE   .or_true@2
   LDA   #$00
   STA   __local_f4__l0_0
   STA   __local_f4__l0_1
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   __local_f4__l0_0
   LDA   #$00
   STA   __local_f4__l0_1
.or_end@3:
   ORA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   __local_f4__l0_2
   STA   __local_f4__l0_3
   LDA   #$00
   STA   __local_f4__it_0
   LDY   #$00
   STY   __local_f4__it_1
.loop@0_start:
   LDA   __local_f4__it_0
   SEC
   SBC   #$03
   LDA   __local_f4__it_1
   SBC   #$00
   BVC   .jcmp_novf@22
   EOR   #$80
.jcmp_novf@22:
   BPL   .loop@0_break
   JSR   f5
   LDA   __local_f4__it_1
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_f4__1
   STA   __local_f4__0
   LDA   HARGS+8
   CLC
   ADC   __local_f4__it_0
   STA   __local_f4__3
   LDA   HARGS+9
   ADC   __local_f4__it_1
   STA   __local_f4__2
   LDA   HARGS+10
   ADC   __local_f4__1
   STA   __local_f4__1
   LDA   HARGS+11
   ADC   __local_f4__0
   STA   __local_f4__0
   LDA   __local_f4__l0_0
   CLC
   ADC   __local_f4__3
   STA   __local_f4__l0_0
   LDA   __local_f4__l0_1
   ADC   __local_f4__2
   STA   __local_f4__l0_1
   LDA   __local_f4__l0_2
   ADC   __local_f4__1
   STA   __local_f4__l0_2
   LDA   __local_f4__l0_3
   ADC   __local_f4__0
   STA   __local_f4__l0_3
   INC   __local_f4__it_0
   BNE   .loop@0_start
   INC   __local_f4__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   AND   g0
   STA   __local_f4__2
   LDA   g0
   BNE   .or_true@4
   LDA   #$00
   STA   __local_f4__1
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   STA   __local_f4__1
.or_end@5:
   LDA   __local_f4__2
   SEC
   SBC   __local_f4__1
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   RTS

f3:
   SUBROUTINE

   LDA   #$00
   STA   __local_f3__it_0
   LDY   #$00
   STY   __local_f3__it_1
.loop@1_start:
   LDA   __local_f3__it_0
   SEC
   SBC   #$03
   LDA   __local_f3__it_1
   SBC   #$00
   BVC   .jcmp_novf@25
   EOR   #$80
.jcmp_novf@25:
   BPL   .loop@1_break
   JSR   f5
   LDA   __local_f3__it_1
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   HARGS+8
   CLC
   ADC   __local_f3__it_0
   STA   __local_f3__3
   LDA   HARGS+9
   ADC   __local_f3__it_1
   STA   __local_f3__2
   LDA   HARGS+10
   ADC   __local_f3__1
   STA   __local_f3__1
   LDA   HARGS+11
   ADC   __local_f3__0
   STA   __local_f3__0
   LDA   __zpabi_f3__p0_0
   CLC
   ADC   __local_f3__3
   STA   __zpabi_f3__p0_0
   LDA   __zpabi_f3__p0_1
   ADC   __local_f3__2
   STA   __zpabi_f3__p0_1
   LDA   __zpabi_f3__p0_2
   ADC   __local_f3__1
   STA   __zpabi_f3__p0_2
   LDA   __zpabi_f3__p0_3
   ADC   __local_f3__0
   STA   __zpabi_f3__p0_3
   INC   __local_f3__it_0
   BNE   .loop@1_start
   INC   __local_f3__it_1
   JMP   .loop@1_start
.loop@1_break:
   LDA   __zpabi_f3__p2_0
   AND   #$07
   TAX
   LDA   #$07
   STA   __local_f3__0
   LDA   #$00
   STA   __local_f3__1
   CPX   #$00
   BEQ   .shift_done@29
.shift_loop@28:
   ASL   __local_f3__0
   ROL   __local_f3__1
   DEX
   BNE   .shift_loop@28
.shift_done@29:
   LDA   __local_f3__1
   ORA   __local_f3__0
   JMP   f3@tail1

f3@tail1:
   SUBROUTINE

   BEQ   .cmp_end@30
   SEC
   RTS
.cmp_end@30:
   CMP   #$01
   RTS

f2:
   SUBROUTINE

   LDA   g0
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_f2__2
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   g0
   SEC
   SBC   __zpabi_f2__p2_0
   LDA   __local_f2__2
   SBC   __zpabi_f2__p2_1
   LDA   __local_f2__1
   SBC   __zpabi_f2__p2_2
   LDA   __local_f2__0
   SBC   __zpabi_f2__p2_3
   LDA   #$00
   ROL
   STA   __local_f2__0
   LDA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f2__0
   JMP   f5@tail0

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p3
   AND   g1
   LSR
   STA   __local_f1__3
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   __local_f1__2
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   __zpabi_f1__p0_0
   SEC
   SBC   __local_f1__3
   LDA   __zpabi_f1__p0_1
   SBC   __local_f1__2
   LDA   __zpabi_f1__p0_2
   SBC   __local_f1__1
   LDA   __zpabi_f1__p0_3
   SBC   __local_f1__0
   BVC   .jcmp_novf@39
   EOR   #$80
.jcmp_novf@39:
   BPL   .and_false@6
   LDA   __zpabi_f1__p0_0
   ORA   __zpabi_f1__p0_1
   ORA   __zpabi_f1__p0_2
   ORA   __zpabi_f1__p0_3
   BNE   .and_end@7
.and_false@6:
.and_end@7:
   LDA   #$01
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   LDA   g0
   BMI   .sx_neg@40
   LDA   #$00
   BEQ   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   STA   __zpabi_f1__p0_1
   LDA   g0
   BMI   .sx_neg@42
   LDA   #$00
   BEQ   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   STA   __zpabi_f1__p2_1
   STA   __zpabi_f1__p2_2
   STA   __zpabi_f1__p2_3
   LDA   g0
   STA   __zpabi_f1__p0_0
   LDA   __zpabi_f1__p0_1
   STA   __zpabi_f1__p0_2
   STA   __zpabi_f1__p0_3
   LDA   __local_f0__p0_0
   STA   __zpabi_f1__p1_0
   STX   __zpabi_f1__p1_1
   LDA   g0
   STA   __zpabi_f1__p2_0
   LDA   #$00
   STA   __zpabi_f1__p3
   JSR   f1
   STA   __local_f0__0
   LDA   __local_f0__p0_0
   EOR   __local_f0__0
   STA   __local_f0__1
   JSR   f4
   STA   __local_f0__0
   LDA   __local_f0__1
   EOR   __local_f0__0
   CLC
   ADC   g0
   STA   g0
   JMP   f3@tail1

main:
   SUBROUTINE

   LDA   #$AD
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   LDX   #$00
   LDA   #$2F
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g0
   BMI   .sx_neg@45
   LDA   #$00
   BEQ   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   __local_main__6
   STA   __local_main__5
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__6
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g1+2
   STA   __local_main__1
   LDA   __local_main__4
   EOR   g1+3
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

g1:
   DC.L  $00000067

__zp_init:
   SUBROUTINE

   LDA   #$42
   STA   g0
   RTS
