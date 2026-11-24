SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$27
z7	EQU	$28
g4	EQU	$29
gp10	EQU	$2B
g1	EQU	$2D
g3	EQU	$31
g2	EQU	$33
__zpabi_f12__p13_0	EQU	$80
__zpabi_f12__p13_1	EQU	$81
__zpabi_f12__p13_2	EQU	$82
__zpabi_f12__p13_3	EQU	$83
__zpabi_f12__p14_0	EQU	$84
__zpabi_f12__p14_1	EQU	$85
__zpabi_f12__p14_2	EQU	$86
__zpabi_f12__p14_3	EQU	$87
__zpabi_mix__v_0	EQU	$88
__zpabi_mix__v_1	EQU	$89
__zpabi_mix__v_2	EQU	$8A
__zpabi_mix__v_3	EQU	$8B
__local_main__1	EQU	$8D
__local_main__i26	EQU	$90
__local_main__l25_1	EQU	$91
__local_main__l25_0	EQU	$92
__local_f18__w24	EQU	$93
__local_f18__0	EQU	$94
__local_f18__1	EQU	$95
__local_f18__l22	EQU	$97
__local_f18__p19	EQU	$98
__local_f12__1	EQU	$9B
__local_f12__2	EQU	$9C
__local_mix__0	EQU	$9E
__local_mix__1	EQU	$9F
__local_mix__2	EQU	$A0
__local_mix__3	EQU	$A1

; @zp-link-meta-begin
; def f12 params=__zpabi_f12__p13_0,__zpabi_f12__p13_1,__zpabi_f12__p13_2,__zpabi_f12__p13_3,__zpabi_f12__p14_0,__zpabi_f12__p14_1,__zpabi_f12__p14_2,__zpabi_f12__p14_3 locals=__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__3 indirect=false in_cycle=false
; def f18 params=__zpabi_f18__p19 locals=__local_f18__w24,__local_f18__0,__local_f18__1,__local_f18__2,__local_f18__l22,__local_f18__p19,__local_f18__3 indirect=false in_cycle=false param_regs=A
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def fin3 params= locals=__local_fin3__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i26,__local_main__l25_1,__local_main__l25_0 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f12 -> mix
; call f18 -> f12
; call f18 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call fin3 -> mix
; call main -> f18
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> fin3
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@tail1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run6 section=.text local=true zero=false
; item fin1@run5 section=.text local=true zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin1@run0@run7 section=.text local=true zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin3 section=.text local=false zero=false
; item f12 section=.text local=false zero=false
; item f12@run3 section=.text local=true zero=false
; item f12@run2 section=.text local=true zero=false
; item f12@run1 section=.text local=true zero=false
; item f18 section=.text local=false zero=false
; item f18@run8 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga8 section=.data local=false zero=false
; item gs9 section=.data local=false zero=false
; item gp11 section=.data local=false zero=false
; item @3.s17 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=1 at=auto def=true init=$03
; zp z6 size=1 at=auto def=true init=$05
; zp z7 size=1 at=auto def=true init=$FF
; zp g4 size=2 at=auto def=true init=$D7,$A0 weight=27
; zp gp10 size=2 at=auto def=true init=<z5,>z5 weight=20
; zp g1 size=4 at=auto def=true init=$03,$00,$00,$00 weight=35
; zp g3 size=2 at=auto def=true init=$FF,$00 weight=13
; zp g2 size=4 at=auto def=true init=$FF,$FF,$00,$00 weight=18
; runtime hargs_size=32 hargs_need=20
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
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JSR   fin0@run4
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run4
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run4
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail1

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin1@run6
   LDA   z5
   JSR   fin1@run5
   LDA   z6
   JSR   fin1@run0
   LDA   z7
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin1@run6
   LDA   z7
   JSR   fin1@run5
   LDA   ga8
   JMP   fin1@tail0

fin1@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run0:
   SUBROUTINE

   JSR   fin1@run0@run7
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run0@run7:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin1@tail0:
   SUBROUTINE

   JSR   fin1@run0@run7
   JMP   fin0@tail1

fin2:
   SUBROUTINE

   LDA   ga8+1
   JSR   fin1@run0
   LDA   ga8+2
   JSR   fin1@run0
   LDA   ga8+3
   JSR   fin1@run0
   LDA   gs9+1
   STA   __zpabi_mix__v_0
   LDA   gs9+2
   STA   __zpabi_mix__v_1
   LDA   gs9+3
   STA   __zpabi_mix__v_2
   LDA   gs9+4
   JMP   fin0@tail1

fin3:
   SUBROUTINE

   LDA   gs9+5
   JMP   fin1@tail0

f12:
   SUBROUTINE

   JSR   f12@run3
   STA   __zpabi_mix__v_1
   JSR   f12@run2
   LDA   __zpabi_mix__v_1
   AND   #$07
   CMP   #$02
   BEQ   .case@1
   CMP   #$07
   BEQ   .case@2
   JMP   .default@7
.case@1:
.case@2:
   LDA   @3.s17
   CLC
   ADC   #$01
   BCC   .carry_inc@3
   INC   @3.s17+1
.carry_inc@3:
   STA   @3.s17
   STA   gs9+5
   LDA   z6
   AND   #$07
   CMP   #$01
   BEQ   .case@4
   CMP   #$04
   BEQ   .case@5
   CMP   #$07
   BNE   .lb_skip@0
   JMP   .case@6
.lb_skip@0:
   JMP   .switch@3_break
.case@4:
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDA   #$06
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   g1
   LDA   HARGS+6
   STA   g1+1
   LDA   HARGS+7
   STA   g1+2
   LDA   HARGS+8
   STA   g1+3
   LDA   HARGS+5
   RTS
.case@5:
   LDA   g3
   SEC
   SBC   #$01
   STA   __local_f12__1
   LDA   g3+1
   SBC   #$00
   STA   __zpabi_mix__v_1
   STA   g3+1
   LDA   __local_f12__1
   STA   g3
   LDY   #$00
   STA   (DPTR),Y
   JSR   f12@run1
   LDA   __zpabi_mix__v_1
   LDY   #$01
   STA   (DPTR),Y
   JSR   f12@run1
   LDA   #$00
   LDY   #$02
   STA   (DPTR),Y
   JSR   f12@run1
   LDA   #$00
   LDY   #$03
   STA   (DPTR),Y
   LDA   z7
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f12__2
   LDY   #$00
   LDA   (gp10),Y
   STA   __local_f12__1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_1
   LDA   z7
   ORA   __local_f12__1
   STA   __local_f12__1
   LDA   __local_f12__2
   ORA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f12__1
   JSR   fin1@run5
.case@6:
   LDA   z5
   BEQ   .if_else@1
   LDA   g3
   STA   __zpabi_mix__v_1
   LDA   g3+1
   STA   __local_f12__1
   LDX   #$05
.shift_loop@10:
   ASL   __zpabi_mix__v_1
   ROL   __local_f12__1
   DEX
   BNE   .shift_loop@10
   LDA   __zpabi_mix__v_1
   STA   g3
   LDA   __local_f12__1
   STA   g3+1
   LDA   #$FF
   EOR   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   LDA   #$FF
   EOR   __local_f12__1
   STA   __zpabi_mix__v_1
   LDA   #$FF
   STA   __zpabi_mix__v_2
   LDA   #$7F
   JSR   fin0@run4
.if_else@1:
.switch@3_break:
.default@7:
   LDA   __zpabi_f12__p14_0
   CLC
   ADC   ga8+1
   RTS

f12@run3:
   SUBROUTINE

   JSR   f12@run1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

f12@run2:
   SUBROUTINE

   LDY   #$01
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   RTS

f12@run1:
   SUBROUTINE

   LDA   gp11
   STA   DPTR
   LDA   gp11+1
   STA   DPTR+1
   RTS

f18:
   SUBROUTINE

   STA   __local_f18__p19
   LDA   #$FF
   STA   __local_f18__l22
   LDA   g4
   STA   __local_f18__0
   LDA   g4+1
   STA   __local_f18__w24
   LDA   #$FF
   EOR   __local_f18__0
   STA   __zpabi_f12__p13_0
   LDA   #$FF
   EOR   __local_f18__w24
   STA   __local_f18__1
   LDA   g4
   STA   __local_f18__0
   LDA   g4+1
   STA   __local_f18__w24
   LDA   #$00
   SEC
   SBC   __local_f18__0
   LDA   #$00
   SBC   __local_f18__w24
   LDA   #$00
   ROL
   STA   __local_f18__w24
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_f12__p14_2
   STA   __zpabi_f12__p14_3
   LDA   __local_f18__1
   STA   __zpabi_f12__p13_1
   LDA   #$00
   STA   __zpabi_f12__p13_2
   STA   __zpabi_f12__p13_3
   LDA   __local_f18__w24
   STA   __zpabi_f12__p14_0
   LDA   #$00
   STA   __zpabi_f12__p14_1
   JSR   f12
   STA   __local_f18__w24
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   JSR   fin1@run6
   LDA   __local_f18__w24
   JSR   fin1@run5
   LDA   ga8+1
   STA   HARGS+4
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   g2+2
   STA   HARGS+2
   LDA   g2+3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   __zpabi_f12__p13_0
   LDA   HARGS+9
   STA   __zpabi_f12__p13_1
   LDA   HARGS+10
   STA   __zpabi_f12__p13_2
   LDA   HARGS+11
   STA   __zpabi_f12__p13_3
   JSR   f12@run3
   STA   __zpabi_f12__p14_0
   JSR   f12@run2
   LDA   #$00
   STA   __zpabi_f12__p14_1
   JSR   f18@run8
   LDA   gs9+1
   STA   __local_f18__w24
   LDA   gs9+2
   STA   __local_f18__0
   LDA   gs9+3
   STA   __local_f18__1
   LDA   gs9+4
   ASL   __local_f18__w24
   ROL   __local_f18__0
   ROL   __local_f18__1
   ROL
   STA   __zpabi_f12__p13_0
   LDA   __local_f18__w24
   STA   gs9+1
   LDA   __local_f18__0
   STA   gs9+2
   LDA   __local_f18__1
   STA   gs9+3
   LDA   __zpabi_f12__p13_0
   STA   gs9+4
   LDA   __local_f18__w24
   ORA   __local_f18__0
   ORA   __local_f18__1
   ORA   __zpabi_f12__p13_0
   BNE   .lb_skip@2
   JMP   .if_end@2
.lb_skip@2:
   LDA   #$00
   STA   __local_f18__w24
.loop@8_continue:
   CLC
   ADC   #$01
   STA   __local_f18__1
   LDA   __local_f18__w24
   CMP   #$04
   BCC   .lb_skip@1
   JMP   .loop@8_break
.lb_skip@1:
   LDY   #$00
   LDA   (gp10),Y
   ADC   #$FF
   STA   (gp10),Y
   AND   #$07
   CMP   #$02
   BNE   .default@11
   LDA   __local_f18__l22
   SEC
   SBC   #$01
   STA   __local_f18__l22
   ORA   __local_f18__p19
   STA   z7
   JMP   .switch@9_break
.default@11:
   LDA   __local_f18__1
   STA   gs9+5
.switch@9_break:
   LDA   g4
   STA   __local_f18__w24
   LDA   g4+1
   ASL   __local_f18__w24
   ROL
   ASL   __local_f18__w24
   ROL
   ASL   __local_f18__w24
   ROL
   ASL   __local_f18__w24
   ROL
   STA   __local_f18__0
   LDA   __local_f18__w24
   STA   g4
   LDA   __local_f18__0
   STA   g4+1
   LDA   __local_f18__w24
   ORA   __local_f18__0
   BEQ   .if_end@3
   LDA   z5
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f18__0
   LDA   z5
   SEC
   SBC   #$08
   BCS   .carry_inc@0
   DEC   __local_f18__0
.carry_inc@0:
   STA   g4
   LDA   __local_f18__0
   STA   g4+1
   LDA   __local_f18__1
   AND   #$03
   TAX
   LDA   ga8,X
   CLC
   ADC   __local_f18__1
   STA   gs9+1
   LDA   #$00
   ADC   #$00
   STA   gs9+2
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   gs9+3
   STA   gs9+4
   JMP   .loop@8_break
.if_end@3:
   LDA   __local_f18__l22
   LDX   #$FF
   SEC
.div_loop@20:
   INX
   SBC   #$05
   BCS   .div_loop@20
   ADC   #$05
   STA   __zpabi_f12__p14_0
   LDA   #$00
   STA   __zpabi_f12__p13_0
   STA   __zpabi_f12__p13_1
   STA   __zpabi_f12__p13_2
   STA   __zpabi_f12__p13_3
   STA   __zpabi_f12__p14_1
   JSR   f18@run8
   LDA   __local_f18__1
   STA   __local_f18__w24
   JMP   .loop@8_continue
.loop@8_break:
.if_end@2:
   LDY   #$00
   LDA   (gp10),Y
   RTS

f18@run8:
   SUBROUTINE

   STA   __zpabi_f12__p14_2
   STA   __zpabi_f12__p14_3
   JMP   f12

main:
   SUBROUTINE

   JSR   f12@run3
   STA   HARGS
   JSR   f12@run2
   LDA   z6
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   JSR   f18
   JSR   fin1@run0
   LDA   #$00
   STA   __local_main__l25_0
   STA   __local_main__l25_1
   STA   __local_main__i26
.loop@12_start:
   LDA   __local_main__i26
   CMP   #$03
   BCS   .loop@12_break
   LDA   __local_main__l25_0
   SEC
   SBC   #$01
   STA   __local_main__l25_0
   BCS   .carry_inc@1
   DEC   __local_main__l25_1
.carry_inc@1:
   LDA   g3
   ORA   __local_main__l25_0
   STA   __local_main__1
   LDA   g3+1
   ORA   __local_main__l25_1
   ORA   __local_main__1
   BEQ   .if_else@5
   LDA   z6
   ORA   #$5A
   STA   z6
   CLC
   ADC   __local_main__i26
   STA   __local_main__l25_0
   LDA   #$00
   ADC   #$00
   STA   __local_main__l25_1
   LDA   g1
   EOR   #$FF
   STA   z5
   CLC
   ADC   #$FF
   STA   g1
   BCC   .carry_inc@2
   INC   g1+1
   BNE   .carry_inc@2
   INC   g1+2
   BNE   .carry_inc@2
   INC   g1+3
.carry_inc@2:
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BNE   .loop@12_break
   BEQ   .if_end@4
.if_else@5:
   LDA   z7
   BNE   .loop@12_break
.if_end@4:
   INC   __local_main__i26
   JMP   .loop@12_start
.loop@12_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   JSR   fin3
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

ga8:
   DC.B  $00, $80, $FF, $00

gs9:
   DS.B  1
   DC.L  $00000008
   DC.B  $FF

gp11:
   DC.W  g2

@3.s17:
   DC.W  $0100

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g2+2
   STA   g2+3
   STA   g3+1
   LDA   #$03
   STA   g1
   STA   z5
   LDA   #$FF
   STA   g2
   STA   g2+1
   STA   g3
   STA   z7
   LDA   #$D7
   STA   g4
   LDA   #$A0
   STA   g4+1
   LDA   #$05
   STA   z6
   LDA   #<z5
   STA   gp10
   LDA   #>z5
   STA   gp10+1
   RTS
