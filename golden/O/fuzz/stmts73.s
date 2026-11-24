SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$2A
g3	EQU	$32
g2	EQU	$34
g1	EQU	$36
__zpabi_f11__p12_0	EQU	$80
__zpabi_f11__p12_1	EQU	$81
__zpabi_f11__p12_2	EQU	$82
__zpabi_f11__p12_3	EQU	$83
__zpabi_f11__p12_4	EQU	$84
__zpabi_f11__p13	EQU	$85
__zpabi_f11__p14	EQU	$86
__zpabi_f11__pp15_0	EQU	$87
__zpabi_f11__pp15_1	EQU	$88
__zpabi_mix__v_0	EQU	$89
__zpabi_mix__v_1	EQU	$8A
__zpabi_mix__v_2	EQU	$8B
__zpabi_mix__v_3	EQU	$8C
__local_main__w24	EQU	$8D
__local_main__0	EQU	$8E
__local_main__1	EQU	$8F
__local_main__4	EQU	$92
__local_main__5	EQU	$93
__local_main__6	EQU	$94
__local_main__l20	EQU	$95
__local_main__w23	EQU	$96
__local_main__l22	EQU	$97
__local_f11__l16	EQU	$9A
__local_f11__3	EQU	$9E
__local_f11__4	EQU	$9F
__local_f11__5	EQU	$A0
__local_f11__6	EQU	$A1
__local_mix__0	EQU	$A2
__local_mix__1	EQU	$A3
__local_mix__2	EQU	$A4
__local_mix__3	EQU	$A5

; @zp-link-meta-begin
; def f11 params=__zpabi_f11__p12_0,__zpabi_f11__p12_1,__zpabi_f11__p12_2,__zpabi_f11__p12_3,__zpabi_f11__p12_4,__zpabi_f11__p13,__zpabi_f11__p14,__zpabi_f11__pp15_0,__zpabi_f11__pp15_1 locals=__local_f11__l16,__local_f11__0,__local_f11__1,__local_f11__2,__local_f11__3,__local_f11__4,__local_f11__5,__local_f11__6 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__w24,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__l20,__local_main__w23,__local_main__l22,__local_main__8,__local_main__9 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f11 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f11
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run5 section=.text local=true zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run1 section=.text local=true zero=false
; item f11 section=.text local=false zero=false
; item f11@run6 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g4 section=.bss local=false zero=true
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item gp10 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=4 at=auto def=true init=$00,$00,$00,$00
; zp z6 size=8 at=auto def=true init=$01,$00,$00,$00,$00,$00,$00,$00
; zp g3 size=2 at=auto def=true init=$FF,$7F weight=139
; zp g2 size=2 at=auto def=true init=$00,$00 weight=130
; zp g1 size=4 at=auto def=true init=$FF,$7F,$00,$00 weight=71
; runtime hargs_size=32 hargs_need=0
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
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run4
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   LDA   z5+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   JSR   fin0@run0
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   LDA   z6+2
   STA   __zpabi_mix__v_2
   LDA   z6+3
   JSR   fin0@run0
   LDA   ga7
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run2
   LDA   ga7
   JSR   fin1@run5
   LDA   ga7+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run2
   LDA   ga7+1
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   ga7+2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin1@run2
   LDA   ga7+2
   JSR   fin1@run5
   LDA   ga7+3
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin1@run2
   LDA   ga7+3
   JSR   fin1@run5
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   LDA   gs8+2
   STA   __zpabi_mix__v_1
   LDA   gs8+3
   STA   __zpabi_mix__v_2
   LDA   gs8+4
   JSR   fin0@run0
   JSR   fin2@run1
   STA   __zpabi_mix__v_3
   JMP   mix

fin2@run1:
   SUBROUTINE

   LDA   gs8+5
   STA   __zpabi_mix__v_0
   LDA   gs8+6
   STA   __zpabi_mix__v_1
   LDA   gs8+7
   STA   __zpabi_mix__v_2
   LDA   gs8+8
   RTS

f11:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f11__l16
   LDA   z6
   ASL
   STA   __local_f11__l16
   LDA   z6+1
   ROL
   STA   __zpabi_mix__v_2
   LDA   z6+2
   ROL
   STA   __zpabi_mix__v_1
   LDA   z6+3
   ROL
   STA   __zpabi_mix__v_0
   LDA   z6+4
   ROL
   STA   __local_f11__3
   LDA   z6+5
   ROL
   STA   __local_f11__4
   LDA   z6+6
   ROL
   STA   __local_f11__5
   LDA   z6+7
   ROL
   STA   __local_f11__6
   LDA   __local_f11__l16
   CLC
   ADC   z6
   STA   z6
   LDA   __zpabi_mix__v_2
   ADC   z6+1
   STA   z6+1
   LDA   __zpabi_mix__v_1
   ADC   z6+2
   STA   z6+2
   LDA   __zpabi_mix__v_0
   ADC   z6+3
   STA   z6+3
   LDA   __local_f11__3
   ADC   z6+4
   STA   z6+4
   LDA   __local_f11__4
   ADC   z6+5
   STA   z6+5
   LDA   __local_f11__5
   ADC   z6+6
   STA   z6+6
   LDA   __local_f11__6
   ADC   z6+7
   STA   z6+7
   JSR   fin2@run1
   JSR   fin0@run0
   LDA   gs8+1
   AND   #$01
   STA   __local_f11__4
   LDA   #$00
   LDY   #$00
   STY   gs8+2
   STY   gs8+3
   STA   gs8+4
   LDA   __local_f11__4
   STA   gs8+1
   LDA   g4
   STA   __zpabi_mix__v_2
   LDA   g4+1
   STA   __local_f11__l16
   LDA   __local_f11__4
   AND   __zpabi_mix__v_2
   STA   __zpabi_mix__v_0
   TYA
   AND   __local_f11__l16
   STA   __zpabi_mix__v_1
   TYA
   JSR   fin0@run4
   LDA   g1
   STA   __local_f11__l16
   LDA   g1+1
   STA   __zpabi_mix__v_0
   LDA   g1+2
   STA   __zpabi_mix__v_1
   LDA   g1+3
   STA   __zpabi_mix__v_2
   INC   __local_f11__l16
   BNE   .inc_done@0
   INC   __zpabi_mix__v_0
   BNE   .inc_done@0
   INC   __zpabi_mix__v_1
   BNE   .inc_done@0
   INC   __zpabi_mix__v_2
.inc_done@0:
   LDA   __local_f11__l16
   STA   g1
   LDA   __zpabi_mix__v_0
   STA   g1+1
   LDA   __zpabi_mix__v_1
   STA   g1+2
   LDA   __zpabi_mix__v_2
   STA   g1+3
   LDA   g1
   STA   __zpabi_mix__v_2
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_0
   LDA   g1+3
   STA   __local_f11__3
   ASL
   ROR   __local_f11__3
   ROR   __zpabi_mix__v_0
   ROR   __zpabi_mix__v_1
   ROR   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_2
   STA   g1
   LDA   __zpabi_mix__v_1
   STA   g1+1
   LDA   __zpabi_mix__v_0
   STA   g1+2
   LDA   __local_f11__3
   STA   g1+3
   LDA   #$01
   ORA   __zpabi_mix__v_2
   ORA   __zpabi_mix__v_1
   ORA   __zpabi_mix__v_0
   ORA   __local_f11__3
   BEQ   .if_else@1
   LDA   z6
   SEC
   SBC   #$0F
   STA   __zpabi_mix__v_0
   LDA   z6+1
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   z6+2
   SBC   #$00
   STA   __zpabi_mix__v_2
   LDA   z6+3
   SBC   #$00
   JSR   fin0@run0
   LDY   #$00
   LDA   (__zpabi_f11__pp15_0),Y
   AND   #$5A
   STA   (__zpabi_f11__pp15_0),Y
   LDA   #$10
   STA   __zpabi_mix__v_0
   JSR   f11@run6
   JMP   .if_end@0
.if_else@1:
   LDA   gs8+5
   LDX   gs8+6
   STX   __local_f11__5
   LDX   gs8+7
   STX   __local_f11__4
   LDX   gs8+8
   STX   __local_f11__3
   LDX   gs8+9
   STX   __zpabi_mix__v_0
   LDX   gs8+10
   STX   __zpabi_mix__v_1
   LDX   gs8+11
   STX   __zpabi_mix__v_2
   LDX   gs8+12
   STX   __local_f11__l16
   ORA   __local_f11__5
   ORA   __local_f11__4
   ORA   __local_f11__3
   ORA   __zpabi_mix__v_0
   ORA   __zpabi_mix__v_1
   ORA   __zpabi_mix__v_2
   ORA   __local_f11__l16
   BEQ   .lnot_true@12
   LDA   #$00
   BEQ   .lnot_end@13
.lnot_true@12:
   LDA   #$01
.lnot_end@13:
   STA   __zpabi_mix__v_1
   LDA   ga7+2
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f11__l16
   LDA   ga7+2
   EOR   #$FF
   STA   __zpabi_mix__v_2
   LDA   __local_f11__l16
   EOR   #$FF
   STA   __local_f11__l16
   LDA   __zpabi_mix__v_1
   SEC
   SBC   __zpabi_mix__v_2
   LDA   #$00
   SBC   __local_f11__l16
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   g2
   LDA   #$00
   STA   g2+1
.if_end@0:
   LDA   g3
   LDX   g3+1
   STX   __zpabi_mix__v_2
   LSR
   LSR
   LSR
   LSR
   STA   __local_f11__l16
   LDA   __zpabi_mix__v_2
   ASL
   ASL
   ASL
   ASL
   ORA   __local_f11__l16
   AND   #$7F
   RTS

f11@run6:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   fin0@run4

main:
   SUBROUTINE

   LDA   #$AE
   STA   __local_main__l20
   LDA   #$73
   STA   __local_main__w23
   LDA   #$0C
   STA   __local_main__l22
   LDY   #$0C
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_main__1
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_main__0
   STA   __local_main__w24
   TYA
   AND   z5
   STA   __zpabi_mix__v_0
   LDA   __local_main__1
   AND   z5+1
   STA   __zpabi_mix__v_1
   LDA   __local_main__0
   AND   z5+2
   STA   __zpabi_mix__v_2
   LDA   __local_main__w24
   AND   z5+3
   JSR   fin0@run0
   LDA   g4
   STA   __local_main__1
   LDA   g4+1
   STA   __zpabi_f11__p12_1
   LDX   #$06
.shift_loop@21:
   ASL   __local_main__1
   ROL   __zpabi_f11__p12_1
   DEX
   BNE   .shift_loop@21
   LDA   __local_main__1
   STA   g4
   LDA   __zpabi_f11__p12_1
   STA   g4+1
   LDA   __local_main__1
   EOR   z5
   AND   #$07
   BNE   .lb_skip@5
   JMP   .switch@5_break
.lb_skip@5:
   CMP   #$02
   BEQ   .case@7
   CMP   #$04
   BNE   .lb_skip@4
   JMP   .case@8
.lb_skip@4:
   CMP   #$06
   BNE   .lb_skip@3
   JMP   .case@9
.lb_skip@3:
   JMP   .default@10
.case@7:
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __zpabi_f11__p12_0
   INY
   LDA   (DPTR),Y
   STA   __zpabi_f11__p12_1
   INY
   LDA   (DPTR),Y
   STA   __local_main__1
   INY
   LDA   (DPTR),Y
   STA   __local_main__0
   INY
   LDA   (DPTR),Y
   STA   __local_main__w24
   INY
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   LDA   __local_main__w24
   AND   #$1F
   LDX   __local_main__1
   STX   __zpabi_f11__p12_2
   LDX   __local_main__0
   STX   __zpabi_f11__p12_3
   EOR   #$10
   SEC
   SBC   #$10
   STA   __zpabi_f11__p12_4
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __zpabi_f11__p13
   INY
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   LDA   g4
   STA   __zpabi_f11__p14
   LDA   g4+1
   JSR   main@run3
   STA   __zpabi_mix__v_0
   JSR   f11@run6
   LDA   g1
   STA   __local_main__w24
   LDA   g1+1
   STA   __zpabi_f11__p12_1
   LDA   g1+2
   STA   __local_main__1
   LDA   g1+3
   STA   __local_main__0
   INC   __local_main__w24
   BNE   .inc_done@1
   INC   __zpabi_f11__p12_1
   BNE   .inc_done@1
   INC   __local_main__1
   BNE   .inc_done@1
   INC   __local_main__0
.inc_done@1:
   LDA   __local_main__w24
   STA   g1
   LDA   __zpabi_f11__p12_1
   STA   g1+1
   LDA   __local_main__1
   STA   g1+2
   LDA   __local_main__0
   STA   g1+3
.case@8:
.case@9:
   LDA   ga7+1
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_main__5
   LDA   __local_main__l20
   STA   __local_main__1
   LDA   __local_main__w23
   LDA   __local_main__l22
   EOR   gs8+1
   STA   __zpabi_f11__p14
   LDA   ga7+1
   STA   __zpabi_f11__p12_0
   LDA   __local_main__5
   STA   __zpabi_f11__p12_1
   STA   __zpabi_f11__p12_2
   STA   __zpabi_f11__p12_3
   STA   __zpabi_f11__p12_4
   LDA   __local_main__1
   STA   __zpabi_f11__p13
   JSR   main@run3
   LDA   z5
   SEC
   SBC   #$5A
   STA   z5
   LDA   z5+1
   SBC   #$00
   STA   z5+1
   LDA   z5+2
   SBC   #$00
   STA   z5+2
   BCC   .lb_skip@2
   JMP   .switch@5_break
.lb_skip@2:
   DEC   z5+3
   JMP   .switch@5_break
.default@10:
   LDA   #$00
   STA   __local_main__w23
.loop@11_start:
   LDA   g3
   STA   __local_main__6
   LDA   g3+1
   STA   __local_main__5
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_main__4
   STA   __zpabi_f11__p12_0
   LDA   z5
   ASL
   STA   __local_main__w24
   LDA   z5+1
   ROL
   STA   __local_main__0
   LDA   z5+2
   ROL
   STA   __local_main__1
   LDA   z5+3
   ROL
   ASL   __local_main__w24
   ROL   __local_main__0
   ROL   __local_main__1
   ROL
   STA   __zpabi_f11__p12_1
   LDA   __local_main__6
   ORA   __local_main__w24
   STA   z5
   LDA   __local_main__5
   ORA   __local_main__0
   STA   z5+1
   LDA   __local_main__4
   ORA   __local_main__1
   STA   z5+2
   LDA   __zpabi_f11__p12_0
   ORA   __zpabi_f11__p12_1
   STA   z5+3
   LDA   __local_main__l22
   AND   #$03
   STA   __local_main__l22
   LDA   #$00
   STA   __local_main__w24
.loop@12_continue:
   CLC
   ADC   #$01
   STA   __local_main__l20
   LDA   __local_main__w24
   BEQ   .lb_skip@1
   JMP   .loop@12_break
.lb_skip@1:
   LDA   g3
   LDX   g3+1
   STX   __local_main__6
   SEC
   SBC   #$01
   STA   __local_main__5
   BCS   .carry_inc@1
   DEC   __local_main__6
.carry_inc@1:
   STA   g3
   LDA   __local_main__6
   STA   g3+1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_main__4
   STA   __zpabi_f11__p12_0
   LDA   #$00
   AND   gs8+5
   STA   __zpabi_f11__p12_1
   LDA   #$00
   AND   gs8+6
   STA   __local_main__1
   LDA   #$00
   AND   gs8+7
   STA   __local_main__0
   LDA   #$00
   AND   gs8+8
   STA   __local_main__w24
   LDA   __local_main__5
   AND   __zpabi_f11__p12_1
   STA   __zpabi_f11__p12_1
   LDA   __local_main__6
   AND   __local_main__1
   STA   __local_main__1
   LDA   __local_main__4
   AND   __local_main__0
   STA   __local_main__0
   LDA   __zpabi_f11__p12_0
   AND   __local_main__w24
   LDX   __zpabi_f11__p12_1
   STX   g1
   LDX   __local_main__1
   STX   g1+1
   LDX   __local_main__0
   STX   g1+2
   STA   g1+3
   LDA   g2+1
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_main__0
   STA   __local_main__w24
   LDA   g2
   ORA   g2+1
   ORA   __local_main__0
   ORA   __local_main__w24
   BEQ   .cond_else@2
   LDA   #$FF
   STA   g2
   JMP   .cond_end@3
.cond_else@2:
   LDA   z6
   STA   g2
   LDA   z6+1
.cond_end@3:
   STA   g2+1
   LDA   __local_main__l20
   STA   __local_main__w24
   JMP   .loop@12_continue
.loop@12_break:
   INC   __local_main__w23
   LDA   __local_main__w23
   CMP   #$04
   BCS   .lb_skip@0
   JMP   .loop@11_start
.lb_skip@0:
.switch@5_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run3:
   SUBROUTINE

   LDA   #<__local_main__l22
   STA   __zpabi_f11__pp15_0
   LDA   #>__local_main__l22
   STA   __zpabi_f11__pp15_1
   JMP   f11

acc:
   DS.B  2

g4:
   DS.B  2

ga7:
   DC.B  $FF, $80, $00, $00

gs8:
   DS.B  1
   DC.L  $80000000
   DC.L  $000000FF
   DC.L  $00000000

gp9:
   DC.W  z5

gp10:
   DC.W  z6

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+2
   STA   g1+3
   STA   g2
   STA   g2+1
   STA   z5
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   z6+1
   STA   z6+2
   STA   z6+3
   STA   z6+4
   STA   z6+5
   STA   z6+6
   STA   z6+7
   LDA   #$FF
   STA   g1
   STA   g3
   LDA   #$7F
   STA   g1+1
   STA   g3+1
   LDA   #$01
   STA   z6
   RTS
