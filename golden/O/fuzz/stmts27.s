SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g2	EQU	$27
gp5	EQU	$29
gp6	EQU	$2B
__zpabi_f17__p19_0	EQU	$80
__zpabi_f17__p19_1	EQU	$81
__zpabi_f7__p9_0	EQU	$82
__zpabi_f7__p9_1	EQU	$83
__zpabi_mix__v_0	EQU	$84
__zpabi_mix__v_1	EQU	$85
__zpabi_mix__v_2	EQU	$86
__zpabi_mix__v_3	EQU	$87
__local_f17__0	EQU	$88
__local_main__w26	EQU	$88
__local_main__0	EQU	$89
__local_main__1	EQU	$8A
__local_main__2	EQU	$8B
__local_f17__l21	EQU	$8D
__local_f17__5	EQU	$8E
__local_f14__2	EQU	$91
__local_f14__3	EQU	$92
__local_f14__l16	EQU	$93
__local_f14__4	EQU	$94
__local_f14__5	EQU	$95
__local_f14__6	EQU	$96
__local_f7__0	EQU	$97
__local_f7__l12	EQU	$98
__local_mix__0	EQU	$9C
__local_mix__1	EQU	$9D
__local_mix__2	EQU	$9E
__local_mix__3	EQU	$9F

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15 locals=__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__l16,__local_f14__4,__local_f14__5,__local_f14__6 indirect=false in_cycle=false param_regs=A
; def f17 params=__zpabi_f17__p18_0,__zpabi_f17__p18_1,__zpabi_f17__p19_0,__zpabi_f17__p19_1 locals=__local_f17__0,__local_f17__1,__local_f17__2,__local_f17__3,__local_f17__4,__local_f17__l21,__local_f17__5 indirect=false in_cycle=false param_regs=A,X,,
; def f7 params=__zpabi_f7__p8,__zpabi_f7__p9_0,__zpabi_f7__p9_1 locals=__local_f7__0,__local_f7__l12,__local_f7__p8,__local_f7__1,__local_f7__2 indirect=false in_cycle=false param_regs=A,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def main params= locals=__local_main__w26,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> f7
; call f17 -> f14
; call f17 -> mix
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call main -> f14
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run1 section=.text local=true zero=false
; item f7 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item f17 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item @3.s13 section=.bss local=true zero=true
; item @11.s23 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$00 weight=8
; zp g2 size=2 at=auto def=true init=$01,$00 weight=11
; zp gp5 size=2 at=auto def=true init=<g2,>g2 weight=9
; zp gp6 size=2 at=auto def=true init=<g1,>g1 weight=7
; runtime hargs_size=32 hargs_need=21
; @zp-link-meta-end

mix:
   SUBROUTINE

   LDA   acc
   ASL
   STA   __local_mix__2
   LDA   acc+1
   ROL
   STA   __local_mix__3
   LDA   #$00
   ROL
   ORA   __local_mix__2
   LDX   __local_mix__3
   STX   __local_mix__0
   EOR   __zpabi_mix__v_0
   STA   __local_mix__1
   LDA   __local_mix__0
   EOR   __zpabi_mix__v_1
   STA   __local_mix__0
   LDA   __local_mix__1
   EOR   __zpabi_mix__v_2
   STA   acc
   LDA   __local_mix__0
   EOR   __zpabi_mix__v_3
   STA   acc+1
   RTS

fin0:
   SUBROUTINE

   LDA   g1
   JSR   fin0@run0
   LDA   g2+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga3
   JSR   fin0@run0
   LDA   ga3+1
   JSR   fin0@run2
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   JSR   fin0@run2
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga3+2
   JSR   fin0@run0
   LDA   ga3+3
   JSR   fin0@run0
   LDA   gs4+1
   STA   __zpabi_mix__v_0
   LDA   gs4+2
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin1@run1
   LDA   gs4+3
   STA   __zpabi_mix__v_0
   LDA   gs4+4
   STA   __zpabi_mix__v_1
   LDA   gs4+5
   STA   __zpabi_mix__v_2
   LDA   gs4+6
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

f7:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$02
   STA   __local_f7__l12
   LDY   #$00
   LDA   (gp6),Y
   AND   __zpabi_mix__v_1
   JSR   fin0@run0
   LDA   __local_f7__l12
   LSR
   LSR
   STA   __local_f7__0
   STA   __local_f7__l12
   LDA   #$00
   SEC
   SBC   __local_f7__0
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   #$01
   SBC   #$00
   STA   __local_f7__l12
   LDA   #$00
   SBC   #$00
   LDX   __local_f7__l12
   STX   __zpabi_mix__v_2
   AND   #$1F
   JSR   fin1@run1
   LDA   g2
   ORA   g2+1
   BNE   .or_true@0
   LDA   @3.s13
   ORA   @3.s13+1
   BNE   .or_true@0
   LDA   #$00
   BEQ   .or_end@1
.or_true@0:
   LDA   #$01
.or_end@1:
   STA   __local_f7__l12
   LDX   #$00
   LDA   #$7F
   RTS

f14:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f14__l16
   STA   __local_f14__4
   LDA   #$00
   STA   __local_f14__5
   STA   __local_f14__6
   LDA   __local_f14__l16
   STA   HARGS
   LDA   __local_f14__4
   STA   HARGS+1
   LDA   __local_f14__5
   STA   HARGS+2
   LDA   __local_f14__6
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   __local_f14__l16
   LDA   HARGS+6
   STA   __local_f14__4
   LDA   HARGS+7
   STA   __local_f14__5
   LDA   HARGS+8
   STA   __local_f14__6
   LDA   HARGS+5
   SEC
   SBC   #$FE
   STA   g1
   BEQ   .if_end@2
   LDY   #$00
   LDA   (gp6),Y
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   (gp6),Y
   LDA   __local_f14__l16
   LDX   __local_f14__4
   LDX   __local_f14__5
   LDX   __local_f14__6
   CLC
   ADC   g2
   LDX   gs4+1
   STX   __zpabi_f7__p9_0
   LDX   gs4+2
   STX   __zpabi_f7__p9_1
   JSR   f7
.if_end@2:
   LDA   gs4+3
   STA   HARGS
   LDA   gs4+4
   STA   HARGS+1
   LDA   gs4+5
   STA   HARGS+2
   LDA   gs4+6
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   STA   gs4+3
   LDA   HARGS+6
   STA   gs4+4
   LDA   HARGS+7
   STA   gs4+5
   LDA   HARGS+8
   STA   gs4+6
   LDY   #$00
   LDA   (gp5),Y
   STA   __local_f14__2
   INY
   LDA   (gp5),Y
   BNE   .thread@0
   LDA   __local_f14__2
   CMP   g1
   BEQ   .cmp_true@4
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   __local_f14__3
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   LDX   __local_f14__3
   STX   HARGS+16
   LDX   #$00
   STX   HARGS+17
   STA   HARGS+18
   STA   HARGS+19
   AND   #$01
   STA   HARGS+20
   RTS

f17:
   SUBROUTINE

   LDA   #$01
   STA   __local_f17__l21
   LDA   #$00
   STA   __local_f17__5
   LDY   #$00
   LDA   (gp6),Y
   JSR   f14
   LDA   HARGS+16
   STA   __local_f17__l21
   LDA   HARGS+17
   STA   __local_f17__5
   LDY   #$00
   LDA   (gp6),Y
   SEC
   SBC   gs4+3
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   gs4+4
   STA   __zpabi_mix__v_1
   LDA   #$00
   SBC   gs4+5
   STA   __zpabi_mix__v_2
   LDA   #$00
   SBC   gs4+6
   AND   #$1F
   JSR   fin1@run1
   LDA   @11.s23
   SEC
   SBC   #$01
   STA   @11.s23
   BCS   .carry_inc@0
   DEC   @11.s23+1
.carry_inc@0:
   INC   ga3
   LDA   __local_f17__l21
   STA   __zpabi_mix__v_2
   LDA   __local_f17__5
   STA   __local_f17__0
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __zpabi_mix__v_2
   STA   HARGS+8
   LDA   __local_f17__0
   STA   HARGS+9
   RTS

main:
   SUBROUTINE

   LDA   #$0B
   STA   __local_main__w26
   LDA   #$00
   STA   __local_main__0
   STA   __local_main__1
   STA   __local_main__2
   LDA   #$FF
   SEC
   SBC   gs4+3
   JSR   f14
   LDY   #$00
   LDA   (gp6),Y
   CLC
   ADC   #$03
   STA   (gp6),Y
   LDA   #$00
   STA   __local_main__w26
.loop@0_start:
   STA   g2
   LDA   #$00
   STA   g2+1
   INC   __local_main__w26
   LDA   __local_main__w26
   BEQ   .loop@0_start
   LDA   g1
   ORA   #$03
   STA   g1
   JSR   fin0@run0
   LDA   g1
   BEQ   .lnot_true@11
   LDA   #$00
   BEQ   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   LDX   #$00
   STX   __zpabi_f7__p9_0
   STX   __zpabi_f7__p9_1
   JSR   f7
   LDA   #$00
   STA   __local_main__w26
   JMP   .loop@1_continue
.main@rot@0:
   LDA   __local_main__1
   AND   #$03
   TAX
   LDA   __local_main__1
   STA   ga3,X
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin1@run1
   LDA   #$07
   SEC
   SBC   __local_main__1
   STA   __local_main__0
   LDA   #$00
   SBC   #$00
   STA   __local_main__w26
   LDA   __local_main__0
   LDY   #$00
   STA   (gp5),Y
   LDA   __local_main__w26
   INY
   STA   (gp5),Y
   LDA   __local_main__1
   STA   __local_main__w26
.loop@1_continue:
   CLC
   ADC   #$01
   STA   __local_main__1
   LDA   __local_main__w26
   CMP   #$02
   BCC   .main@rot@0
   JSR   fin0
   JSR   fin1
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

ga3:
   DC.B  $03, $FF, $7F, $00

gs4:
   DS.B  1
   DC.W  $007F
   DC.L  $00000001

@3.s13:
   DS.B  2

@11.s23:
   DC.W  $0002

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g2+1
   LDA   #$01
   STA   g2
   LDA   #<g2
   STA   gp5
   LDA   #>g2
   STA   gp5+1
   LDA   #<g1
   STA   gp6
   LDA   #>g1
   STA   gp6+1
   RTS
