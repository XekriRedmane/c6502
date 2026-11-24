SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$2A
z6	EQU	$2C
g1	EQU	$30
__local_f10__0	EQU	$80
__zpabi_f12__p13_0	EQU	$80
__local_f10__1	EQU	$81
__zpabi_f12__p13_1	EQU	$81
__local_f10__2	EQU	$82
__zpabi_f12__p13_2	EQU	$82
__zpabi_f12__p13_3	EQU	$83
__zpabi_f12__p13_4	EQU	$84
__zpabi_f12__p13_5	EQU	$85
__zpabi_f12__p13_6	EQU	$86
__zpabi_f12__p13_7	EQU	$87
__zpabi_mix__v_0	EQU	$88
__zpabi_mix__v_1	EQU	$89
__zpabi_mix__v_2	EQU	$8A
__zpabi_mix__v_3	EQU	$8B
__local_f12__0	EQU	$8C
__local_main__0	EQU	$8C
__local_f12__1	EQU	$8D
__local_main__w18	EQU	$8D
__local_f12__2	EQU	$8E
__local_main__1	EQU	$8E
__local_f12__3	EQU	$8F
__local_main__2	EQU	$8F
__local_f12__4	EQU	$90
__local_main__3	EQU	$90
__local_f12__5	EQU	$91
__local_main__4	EQU	$91
__local_f12__6	EQU	$92
__local_main__5	EQU	$92
__local_main__6	EQU	$93
__local_f12__8	EQU	$94
__local_f12__9	EQU	$95
__local_f12__10	EQU	$96
__local_f12__11	EQU	$97
__local_mix__0	EQU	$98
__local_mix__1	EQU	$99
__local_mix__2	EQU	$9A
__local_mix__3	EQU	$9B

; @zp-link-meta-begin
; def f10 params= locals=__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3 indirect=false in_cycle=false
; def f12 params=__zpabi_f12__p13_0,__zpabi_f12__p13_1,__zpabi_f12__p13_2,__zpabi_f12__p13_3,__zpabi_f12__p13_4,__zpabi_f12__p13_5,__zpabi_f12__p13_6,__zpabi_f12__p13_7 locals=__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__3,__local_f12__4,__local_f12__5,__local_f12__6,__local_f12__7,__local_f12__8,__local_f12__9,__local_f12__10,__local_f12__11 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__w18,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f12 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f10
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f10@run1 section=.text local=true zero=false
; item f12 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=4 at=auto def=true init=$04,$00,$00,$00
; zp z5 size=2 at=auto def=true init=$FF,$FF
; zp z6 size=4 at=auto def=true init=$09,$00,$00,$00
; zp g1 size=1 at=auto def=true init=$00 weight=3
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run3
   LDA   g3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run2
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   JSR   fin0@run0
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run2
   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin0@run0
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   LDA   z6+2
   STA   __zpabi_mix__v_2
   LDA   z6+3
   JSR   fin0@run3
   LDA   ga7+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run2
   LDA   ga7
   STA   __zpabi_mix__v_0
   LDA   ga7+1
   JSR   fin0@run0
   LDA   ga7+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run2
   LDA   ga7+2
   STA   __zpabi_mix__v_0
   LDA   ga7+3
   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+5
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run2
   LDA   ga7+4
   STA   __zpabi_mix__v_0
   LDA   ga7+5
   JSR   fin0@run0
   LDA   ga7+7
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   JSR   fin0@run2
   LDA   ga7+6
   STA   __zpabi_mix__v_0
   LDA   ga7+7
   JSR   fin0@run0
   LDA   gs8+1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs8+2
   STA   __zpabi_mix__v_0
   LDA   gs8+3
   STA   __zpabi_mix__v_1
   LDA   gs8+4
   STA   __zpabi_mix__v_2
   LDA   gs8+5
   STA   __zpabi_mix__v_3
   JMP   mix

f10:
   SUBROUTINE

   LDA   gs8+2
   LDX   gs8+3
   STX   __local_f10__2
   LDX   gs8+4
   STX   __local_f10__1
   LDX   gs8+5
   STX   __local_f10__0
   ASL
   LDA   __local_f10__2
   ROL
   STA   gs8+2
   LDA   __local_f10__1
   ROL
   STA   gs8+3
   LDA   __local_f10__0
   ROL
   STA   gs8+4
   LDA   #$00
   ROL
   STA   gs8+5
   JSR   f10@run1
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   LDY   #$00
   STA   (DPTR),Y
   AND   #$07
   CMP   #$01
   BEQ   .case@1
   CMP   #$03
   BEQ   .vthread@0
   BNE   .switch@0_break
.case@1:
   LDA   z5
   ORA   g3
   STA   gs8+1
   JMP   .switch@0_break
.vthread@0:
   LDA   z6
   ORA   #$03
   STA   z6
   CLC
   ADC   g1
   RTS
.switch@0_break:
   LDA   #$00
   STA   z5+1
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   JSR   fin0@run2
   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   #$00
   JSR   fin0@run0
   LDA   z4
   LDX   z4+1
   LDX   z4+2
   LDX   z4+3
   CLC
   ADC   #$FF
   RTS

f10@run1:
   SUBROUTINE

   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

f12:
   SUBROUTINE

   LDA   #$33
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   JSR   mix
   LDA   z6
   EOR   #$05
   STA   z6
   SEC
   SBC   #$80
   STA   __local_f12__11
   LDA   z6+1
   SBC   #$00
   STA   __local_f12__10
   LDA   z6+2
   SBC   #$00
   STA   __local_f12__9
   LDA   z6+3
   SBC   #$00
   STA   __local_f12__8
   LDA   z4
   LDX   z4+1
   STX   __local_f12__6
   LDX   z4+2
   STX   __local_f12__5
   LDX   z4+3
   STX   __local_f12__4
   SEC
   SBC   g2
   STA   __local_f12__3
   LDA   __local_f12__6
   SBC   g2+1
   STA   __local_f12__2
   LDA   __local_f12__5
   SBC   g2+2
   STA   __local_f12__1
   LDA   __local_f12__4
   SBC   g2+3
   STA   __local_f12__0
   LDA   __local_f12__11
   CLC
   ADC   __local_f12__3
   STA   __zpabi_mix__v_0
   LDA   __local_f12__10
   ADC   __local_f12__2
   STA   __zpabi_mix__v_1
   LDA   __local_f12__9
   ADC   __local_f12__1
   STA   __zpabi_mix__v_2
   LDA   __local_f12__8
   ADC   __local_f12__0
   JSR   fin0@run3
   JSR   f10@run1
   SEC
   SBC   #$01
   LDY   #$00
   STA   (DPTR),Y
   STA   ga7+6
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   ga7+7
   LDA   #$81
   STA   z4
   LDA   #$00
   STA   z4+1
   STA   z4+2
   STA   z4+3
   LDX   __zpabi_f12__p13_1
   LDA   __zpabi_f12__p13_0
   RTS

main:
   SUBROUTINE

   LDA   ga7+2
   EOR   #$03
   STA   ga7+2
   LDA   g2
   SEC
   SBC   gs8+1
   AND   #$07
   CMP   #$01
   BNE   .lb_skip@1
   JMP   .switch@6_break
.lb_skip@1:
   CMP   #$03
   BEQ   .lb_skip@0
   JMP   .switch@6_break
.lb_skip@0:
   LDA   ga7
   STA   __local_main__2
   LDA   ga7+1
   STA   __local_main__1
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_main__w18
   STA   __local_main__0
   LDA   z6
   ORA   #$01
   STA   z6
   LDA   __local_main__2
   EOR   z6
   STA   __local_main__2
   LDA   __local_main__1
   EOR   z6+1
   STA   __local_main__1
   LDA   __local_main__w18
   EOR   z6+2
   STA   __local_main__w18
   LDA   __local_main__0
   EOR   z6+3
   STA   __local_main__0
   LDA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__w18
   ORA   __local_main__0
   BEQ   .if_end@1
   LDA   ga7+2
   SEC
   SBC   #$81
   STA   ga7+2
   BCS   .carry_inc@0
   DEC   ga7+3
.carry_inc@0:
   LDA   z4
   STA   __local_main__6
   LDA   z4+1
   STA   __local_main__5
   LDA   z4+2
   STA   __local_main__4
   LDA   z4+3
   STA   __local_main__3
   LDA   g1
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_main__1
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __local_main__w18
   STA   __local_main__0
   LDA   __local_main__6
   CLC
   ADC   g1
   STA   gs8+2
   LDA   __local_main__5
   ADC   __local_main__1
   STA   gs8+3
   LDA   __local_main__4
   ADC   __local_main__w18
   STA   gs8+4
   LDA   __local_main__3
   ADC   __local_main__0
   STA   gs8+5
.if_end@1:
.switch@6_break:
   LDA   #$00
   STA   __local_main__w18
   JMP   .loop@9_continue
.main@rot@0:
   LDA   #$03
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   JSR   f10
   LDA   __local_main__0
   STA   __local_main__w18
.loop@9_continue:
   CLC
   ADC   #$01
   STA   __local_main__0
   LDA   __local_main__w18
   CMP   #$03
   BCC   .main@rot@0
   LDA   gs8+2
   CLC
   ADC   #$03
   STA   gs8+2
   BCC   .carry_inc@1
   INC   gs8+3
   BNE   .carry_inc@1
   INC   gs8+4
   BNE   .carry_inc@1
   INC   gs8+5
.carry_inc@1:
   JSR   f10
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

g2:
   DC.L  $04D66FAE
   DC.L  $E608FFAF

g3:
   DC.W  $8000

ga7:
   DS.B  4
   DC.W  $0002
   DC.W  $FFFF

gs8:
   DS.B  1
   DC.B  $FF
   DC.L  $80000000

gp9:
   DC.W  g1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z6+1
   STA   z6+2
   STA   z6+3
   LDA   #$04
   STA   z4
   LDA   #$FF
   STA   z5
   STA   z5+1
   LDA   #$09
   STA   z6
   RTS
