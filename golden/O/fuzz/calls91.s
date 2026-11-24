SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
__local_main__0	EQU	$80
__zpabi_f1__p1_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f1__p1_1	EQU	$81
__local_main__2	EQU	$82
__zpabi_f1__p2_0	EQU	$82
__local_main__3	EQU	$83
__zpabi_f1__p2_1	EQU	$83
__local_main__4	EQU	$84
__zpabi_f1__p2_2	EQU	$84
__local_main__5	EQU	$85
__zpabi_f2__p0_0	EQU	$85
__local_main__6	EQU	$86
__zpabi_f2__p0_1	EQU	$86
__zpabi_f2__p0_2	EQU	$87
__zpabi_f2__p0_3	EQU	$88
__zpabi_f2__p1_0	EQU	$89
__zpabi_f2__p1_1	EQU	$8A
__zpabi_f2__p1_2	EQU	$8B
__zpabi_f2__p2_0	EQU	$8C
__zpabi_f2__p2_1	EQU	$8D
__local_f0__p0_1	EQU	$8E
__zpabi_f2__p2_2	EQU	$8E
__local_f0__0	EQU	$8F
__zpabi_f2__p2_3	EQU	$8F
__local_f0__p0_0	EQU	$90
__local_f2__0	EQU	$94
__local_f2__1	EQU	$95
__local_f2__2	EQU	$96
__local_f2__3	EQU	$97

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1 locals=__local_f0__p0_1,__local_f0__0,__local_f0__p0_0,__local_f0__1,__local_f0__2 indirect=false in_cycle=false param_regs=A,X
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false param_regs=A,X,,,,,
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f1 -> f2
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f1@tail0 section=.text local=true zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$66,$00 weight=9
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f2:
   SUBROUTINE

   LDA   g1+1
   BMI   .jcmp_neg@0
   ORA   g1
   BNE   .and_false@0
.jcmp_neg@0:
   LDA   g2
   AND   #$07
   TAX
   LDA   g1
   STA   __local_f2__0
   LDA   g1+1
   STA   __local_f2__1
   CPX   #$00
   BEQ   .shift_done@2
.shift_loop@1:
   LDA   __local_f2__1
   ASL
   ROR   __local_f2__1
   ROR   __local_f2__0
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   LDA   __local_f2__0
   ORA   __local_f2__1
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f2__3
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f2__3
.and_end@1:
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@2
   LDA   g2
   ORA   g2+1
   ORA   g2+2
   BNE   .and_end@3
.and_false@2:
.and_end@3:
   LDA   g1
   EOR   __local_f2__3
   STA   __local_f2__3
   LDA   __zpabi_f2__p0_0
   AND   #$07
   STA   __local_f2__2
   LDA   __local_f2__3
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f2__0
   LDA   __local_f2__2
   SEC
   SBC   __local_f2__3
   LDA   #$00
   SBC   __local_f2__0
   LDA   #$00
   ROL
   RTS

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p2_0
   CLC
   ADC   __zpabi_f1__p1_0
   STA   __zpabi_f2__p1_1
   LDA   __zpabi_f1__p2_1
   ADC   __zpabi_f1__p1_1
   STA   __zpabi_f2__p1_2
   LDA   __zpabi_f1__p2_2
   ADC   #$00
   STA   __zpabi_f2__p2_0
   LDA   __zpabi_f2__p1_1
   STA   __zpabi_f2__p1_0
   LDA   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_1
   LDA   __zpabi_f2__p2_0
   AND   #$1F
   STA   __zpabi_f2__p1_2
   LDA   __zpabi_f1__p1_0
   CLC
   SBC   g2
   LDA   __zpabi_f1__p1_1
   SBC   g2+1
   LDA   #$00
   SBC   g2+2
   LDA   #$00
   ROL
   STA   __zpabi_f2__p2_0
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __zpabi_f2__p2_2
   STA   __zpabi_f2__p2_3
   LDA   #$03
   STA   __zpabi_f2__p0_0
   LDA   #$00
   STA   __zpabi_f2__p0_1
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   STA   __zpabi_f2__p2_1
   JSR   f2
   STA   __zpabi_f2__p2_0
   LDA   #$01
   EOR   __zpabi_f2__p2_0
   STA   __zpabi_f2__p1_1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __zpabi_f2__p1_2
   AND   #$07
   STA   __zpabi_f2__p2_0
   LDA   g2
   CLC
   ADC   __zpabi_f2__p1_1
   STA   __zpabi_f2__p1_1
   LDA   g2+1
   ADC   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_2
   LDA   g2+2
   ADC   __zpabi_f2__p2_0
   STA   __zpabi_f2__p2_0
   LDA   __zpabi_f2__p1_1
   STA   g2
   LDA   __zpabi_f2__p1_2
   STA   g2+1
   LDA   __zpabi_f2__p2_0
   AND   #$07
   STA   g2+2
   LDA   __zpabi_f1__p2_2
   CMP   __zpabi_f1__p2_2
   BNE   .thread@0
   LDA   __zpabi_f1__p2_1
   CMP   __zpabi_f1__p2_1
   BNE   .thread@0
   LDA   __zpabi_f1__p2_0
   CMP   __zpabi_f1__p2_0
   BEQ   .cmp_true@11
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   STA   __zpabi_f2__p2_0
   LDA   #$00
   BNE   .thread@1
   LDA   #$01
   CMP   __zpabi_f2__p2_0
   BEQ   .cmp_true@14
.thread@1:
   LDA   #$00
.cmp_true@14:
   STA   __zpabi_f2__p2_0
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __zpabi_f2__p2_0
   JMP   f1@tail0

f1@tail0:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f0:
   SUBROUTINE

   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   __local_f0__0
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_f0__p0_0
   LDA   #$00
   CMP   g2+2
   BNE   .vthread@0
   CMP   g2+1
   BNE   .vthread@0
   LDA   #$01
   CMP   g2
   BEQ   .cmp_true@21
   LDA   #$00
.vthread@0:
.cmp_true@21:
   STA   __local_f0__p0_1
   LDA   g1+1
   BNE   .thread@3
   LDA   g1
   CMP   __local_f0__p0_1
   BEQ   .cmp_true@24
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   STA   __local_f0__p0_1
   ORA   #$01
   STA   HARGS+1
   LDA   __local_f0__p0_1
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+2
   BNE   .or_true@8
   LDA   __local_f0__0
   EOR   __local_f0__0
   STA   __local_f0__0
   LDA   #$00
   EOR   __local_f0__p0_0
   ORA   __local_f0__0
   BNE   .or_true@8
   LDA   #$00
   STA   __local_f0__0
   JMP   .or_end@9
.or_true@8:
   LDA   #$01
   STA   __local_f0__0
   LDA   #$00
.or_end@9:
   ORA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f0__0
   JMP   f1@tail0

main:
   SUBROUTINE

   LDX   #$01
   LDA   #$25
   JSR   f0
   LDA   g0+1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g1+1
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__6
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__5
   LDA   __local_main__4
   EOR   __local_main__0
   STA   __local_main__4
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__6
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g2+2
   STA   __local_main__1
   LDA   __local_main__4
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

g0:
   DC.W  $00AF

g2:
   DC.B  $C8
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   LDA   #$66
   STA   g1
   RTS
