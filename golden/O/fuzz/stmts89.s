SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g3	EQU	$27
g2	EQU	$29
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_f13__0	EQU	$84
__local_f13__1	EQU	$85
__local_f13__2	EQU	$86
__local_f13__5	EQU	$89
__local_f13__6	EQU	$8A
__local_f8__w12	EQU	$8B
__local_f8__0	EQU	$8C
__local_mix__0	EQU	$8F
__local_mix__1	EQU	$90
__local_mix__2	EQU	$91
__local_mix__3	EQU	$92

; @zp-link-meta-begin
; def f13 params= locals=__local_f13__0,__local_f13__1,__local_f13__2,__local_f13__3,__local_f13__4,__local_f13__5,__local_f13__6 indirect=false in_cycle=false
; def f8 params=__zpabi_f8__p9 locals=__local_f8__w12,__local_f8__0,__local_f8__1,__local_f8__2 indirect=false in_cycle=false param_regs=A
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f13 -> f8
; call f13 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run4 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f8@run3 section=.text local=true zero=false
; item f13 section=.text local=false zero=false
; item f13@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$01
; zp g3 size=2 at=auto def=true init=$FF,$FF weight=22
; zp g2 size=5 at=auto def=true init=$FF,$FF,$00,$00,$00 weight=24
; runtime hargs_size=32 hargs_need=9
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
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run2
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   JSR   fin0@run0
   LDA   z4
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z4
   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run4
   JMP   mix

fin0@run0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   JSR   fin0@run0
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin0@run0
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin0@run0
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   JSR   fin0@run0@run4
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   JSR   fin0@run2
   LDA   gs6+5
   STA   __zpabi_mix__v_0
   LDA   gs6+6
   STA   __zpabi_mix__v_1
   LDA   gs6+7
   STA   __zpabi_mix__v_2
   LDA   gs6+8
   STA   __zpabi_mix__v_3
   JMP   mix

f8:
   SUBROUTINE

   INC   g1
   BNE   .inc_done@0
   INC   g1+1
.inc_done@0:
   LDA   g1
   AND   #$07
   CMP   #$02
   BEQ   .case@1
   CMP   #$03
   BEQ   .case@3
   CMP   #$06
   BEQ   .case@4
   BNE   .default@5
.case@1:
   LDA   #$00
   STA   __local_f8__w12
.loop@2_start:
   INC   g3
   BNE   .inc_done@1
   INC   g3+1
.inc_done@1:
   LDA   g2
   EOR   #$FF
   STA   g2
   INC   __local_f8__w12
   LDA   __local_f8__w12
   CMP   #$03
   BCC   .loop@2_start
.case@3:
   LDA   z4
   LSR
   LDA   #$00
   ROR
   CLC
   ADC   z4
   STA   __local_f8__0
   STA   z4
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f8__w12
   LDA   __local_f8__0
   EOR   #$02
   STA   __local_f8__0
   LDA   __local_f8__w12
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f8__0
   STA   __zpabi_mix__v_0
   LDA   __local_f8__w12
   JSR   f8@run3
   JMP   .switch@0_break
.case@4:
.default@5:
   LDA   gs6+5
   STA   __zpabi_mix__v_3
   LDA   gs6+6
   STA   __zpabi_mix__v_2
   LDA   gs6+7
   STA   __local_f8__0
   LDA   gs6+8
   STA   __local_f8__w12
   LDA   __zpabi_mix__v_3
   EOR   #$5A
   STA   gs6+5
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_1
   LDA   __local_f8__0
   STA   __zpabi_mix__v_2
   LDA   __local_f8__w12
   JSR   fin0@run2
   LDA   gs6+1
   AND   #$5A
   STA   gs6+1
   LDA   #$00
   STA   gs6+2
   LDY   #$00
   STY   gs6+3
   STA   gs6+4
.switch@0_break:
   LDA   z4
   ASL
   ASL
   ASL
   STA   __zpabi_mix__v_3
   STA   z4
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __local_f8__0
   AND   #$0F
   STA   __local_f8__w12
   LDA   #$01
   EOR   __zpabi_mix__v_3
   STA   __zpabi_mix__v_0
   LDA   #$00
   EOR   __zpabi_mix__v_2
   STA   __zpabi_mix__v_1
   LDA   #$00
   EOR   __local_f8__0
   STA   __zpabi_mix__v_2
   LDA   #$00
   EOR   __local_f8__w12
   JSR   fin0@run2
   LDA   gs6+1
   STA   HARGS
   LDA   gs6+2
   STA   HARGS+1
   LDA   gs6+3
   STA   HARGS+2
   LDA   gs6+4
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   __local_f8__w12
   LDA   HARGS+6
   STA   __local_f8__0
   LDA   HARGS+8
   STA   gs6+4
   LDA   HARGS+7
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   ASL   __local_f8__w12
   ROL   __local_f8__0
   ROL   __zpabi_mix__v_2
   ROL
   STA   __zpabi_mix__v_3
   LDA   __local_f8__w12
   STA   gs6+1
   LDA   __local_f8__0
   STA   gs6+2
   LDA   __zpabi_mix__v_2
   STA   gs6+3
   LDA   __zpabi_mix__v_3
   STA   gs6+4
   LDA   #$FF
   ORA   __local_f8__w12
   STA   __zpabi_mix__v_0
   LDA   __local_f8__0
   JSR   f8@run3
   LDA   g1
   CLC
   ADC   #$81
   STA   g1
   BCC   .carry_inc@0
   INC   g1+1
.carry_inc@0:
   LDA   #$01
   SEC
   SBC   g3
   STA   __local_f8__w12
   LDA   #$00
   SBC   g3+1
   STA   __local_f8__0
   LDA   __local_f8__w12
   LDX   __local_f8__0
   RTS

f8@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

f13:
   SUBROUTINE

   LDA   z4
   AND   z4
   JSR   f8
   LDA   g2
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_2
   LDA   g2+1
   SBC   #$00
   STA   __zpabi_mix__v_3
   LDA   g2+2
   SBC   #$00
   STA   __local_f13__2
   LDA   g2+3
   SBC   #$00
   STA   __local_f13__1
   LDA   g2+4
   SBC   #$00
   LDX   __zpabi_mix__v_2
   STX   g2
   LDX   __zpabi_mix__v_3
   STX   g2+1
   LDX   __local_f13__2
   STX   g2+2
   LDX   __local_f13__1
   STX   g2+3
   AND   #$01
   STA   g2+4
   LDA   g2
   ORA   g2+1
   ORA   g2+2
   ORA   g2+3
   ORA   g2+4
   BEQ   .if_else@1
   LDA   ga5
   LDX   gp7
   STX   DPTR
   LDX   gp7+1
   STX   DPTR+1
   LDY   #$00
   STA   (DPTR),Y
   JSR   f13@run1
   SEC
   SBC   #$FF
   LDY   #$00
   STA   (DPTR),Y
   JMP   .if_end@0
.if_else@1:
   LDA   z4
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_3
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f13__2
   STA   __local_f13__1
   LDA   #$01
   CLC
   ADC   z4
   STA   __zpabi_mix__v_2
   LDA   #$00
   ADC   __zpabi_mix__v_3
   STA   __zpabi_mix__v_3
   LDA   #$00
   ADC   __local_f13__2
   STA   __local_f13__2
   LDA   #$00
   ADC   __local_f13__1
   LDX   __zpabi_mix__v_2
   STX   __local_f13__6
   LDX   __zpabi_mix__v_3
   STX   __local_f13__5
   LDX   __local_f13__2
   STX   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+1
   STA   __local_f13__1
   LDA   gs6+2
   STA   __local_f13__0
   LDA   __local_f13__6
   ORA   __local_f13__1
   STA   __zpabi_mix__v_0
   LDA   __local_f13__5
   ORA   __local_f13__0
   JSR   f8@run3
   LDA   g3
   CLC
   ADC   #$7F
   AND   #$07
   BEQ   .case@7
   CMP   #$01
   BEQ   .case@8
   BNE   .default@9
.case@7:
   LDA   ga5+4
   LDX   ga5+5
   STX   __local_f13__0
   ORA   #$5A
   STA   ga5+4
   STA   g3
   LDA   __local_f13__0
   STA   g3+1
   LDA   gs6+5
   SEC
   SBC   #$01
   STA   gs6+5
   LDA   gs6+6
   SBC   #$00
   STA   gs6+6
   LDA   gs6+7
   SBC   #$00
   STA   gs6+7
   BCS   .carry_inc@1
   DEC   gs6+8
.carry_inc@1:
.case@8:
.default@9:
.if_end@0:
   LDA   z4
   CLC
   ADC   #$FF
   RTS

f13@run1:
   SUBROUTINE

   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

main:
   SUBROUTINE

   JSR   f13@run1
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run2
   INC   g3
   BNE   .inc_done@2
   INC   g3+1
.inc_done@2:
   LDA   g2
   AND   #$01
   STA   g2
   LDA   #$00
   STA   g2+1
   LDY   #$00
   STY   g2+2
   STY   g2+3
   STA   g2+4
   LDA   g2
   EOR   #$06
   STA   ga5+4
   STY   ga5+5
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

g1:
   DC.W  $FFFF

ga5:
   DC.W  $0080
   DC.W  $FFFF
   DC.W  $0001
   DC.W  $007F

gs6:
   DS.B  1
   DC.L  $00008000
   DC.L  $0000000B

gp7:
   DC.W  z4

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+2
   STA   g2+3
   STA   g2+4
   LDA   #$FF
   STA   g2
   STA   g2+1
   STA   g3
   STA   g3+1
   LDA   #$01
   STA   z4
   RTS
