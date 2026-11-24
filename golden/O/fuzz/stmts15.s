SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
g4	EQU	$27
g3	EQU	$28
g1	EQU	$2A
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__l18	EQU	$84
__local_main__0	EQU	$85
__local_f9__1	EQU	$86
__local_main__1	EQU	$86
__local_f9__2	EQU	$87
__local_main__2	EQU	$87
__local_f9__3	EQU	$88
__local_f13__1	EQU	$89
__local_f13__2	EQU	$8A
__local_f13__i17	EQU	$8B
__local_f13__l16	EQU	$8C
__local_f13__p14	EQU	$8D
__local_mix__0	EQU	$8E
__local_mix__1	EQU	$8F
__local_mix__2	EQU	$90
__local_mix__3	EQU	$91

; @zp-link-meta-begin
; def f13 params=__zpabi_f13__p14 locals=__local_f13__0,__local_f13__1,__local_f13__2,__local_f13__i17,__local_f13__l16,__local_f13__p14 indirect=false in_cycle=false param_regs=A
; def f9 params= locals=__local_f9__w11,__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__l18,__local_main__0,__local_main__1,__local_main__2 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f13 -> mix
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f13
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run6 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin0@tail0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run7 section=.text local=true zero=false
; item fin1@run5 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run8 section=.text local=true zero=false
; item f9 section=.text local=false zero=false
; item f13 section=.text local=false zero=false
; item f13@run4 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=1 at=auto def=true init=$80
; zp g4 size=1 at=auto def=true init=$01 weight=18
; zp g3 size=2 at=auto def=true init=$00,$00 weight=18
; zp g1 size=2 at=auto def=true init=$4E,$AA weight=12
; runtime hargs_size=32 hargs_need=12
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

   LDA   g1+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run3
   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   JSR   fin0@run6
   JSR   fin0@run1
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
   JSR   fin0@run3
   JSR   mix
   LDA   g4
   JMP   fin0@tail0

fin0@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   JSR   fin0@tail0@run0
   JMP   mix

fin0@tail0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run3
   RTS

fin1:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run7
   LDA   z5
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga6
   JSR   fin1@run5
   LDA   ga6+1
   JSR   fin1@run5
   LDA   ga6+2
   JMP   fin0@tail0

fin1@run7:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin0@run3

fin1@run5:
   SUBROUTINE

   JSR   fin0@tail0@run0
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+3
   JSR   fin1@run5
   LDA   gs7+2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run3
   JSR   fin2@run8
   JSR   fin0@run6
   LDA   gs7+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin1@run7
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   JMP   mix

fin2@run8:
   SUBROUTINE

   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   RTS

f9:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f9__3
   LDA   __zpabi_mix__v_1
   CMP   #$04
   BCS   .loop@0_break
   LDA   g3
   STA   __zpabi_mix__v_1
   LDA   g3+1
   STA   __local_f9__2
   LDA   __zpabi_mix__v_1
   SEC
   SBC   #$05
   STA   __local_f9__1
   BCS   .carry_inc@0
   DEC   __local_f9__2
.carry_inc@0:
   STA   g3
   LDA   __local_f9__2
   STA   g3+1
   LDA   __local_f9__3
   BEQ   .and_false@0
   LDA   gs7+1
   ORA   gs7+2
   BEQ   .and_false@0
   LDA   #$01
   STA   __zpabi_mix__v_0
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __zpabi_mix__v_0
.and_end@1:
   LDA   __local_f9__1
   SEC
   SBC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   __local_f9__2
   SBC   #$00
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run3
   JSR   mix
   LDA   __local_f9__3
   STA   __zpabi_mix__v_1
   JMP   .loop@0_continue
.loop@0_break:
   LDA   z5
   EOR   #$01
   STA   z5
   LDA   g3
   STA   __local_f9__2
   LDA   g3+1
   STA   __local_f9__1
   LDA   g1
   ASL
   STA   __zpabi_mix__v_1
   LDA   g1+1
   ROL
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   CLC
   ADC   g1
   STA   g1
   LDA   __zpabi_mix__v_0
   ADC   g1+1
   STA   g1+1
   LDA   __local_f9__2
   AND   g1
   STA   __zpabi_mix__v_0
   LDA   __local_f9__1
   AND   g1+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __zpabi_mix__v_0
   STA   HARGS+8
   LDA   __zpabi_mix__v_1
   STA   HARGS+9
   RTS

f13:
   SUBROUTINE

   STA   __local_f13__p14
   LDA   #$00
   STA   __local_f13__l16
   LDA   gs7+3
   CLC
   ADC   #$01
   JSR   f13@run4
   STA   gs7+3
   LDA   g4
   AND   #$07
   BEQ   .case@3
   CMP   #$02
   BEQ   .case@4
   CMP   #$03
   BEQ   .case@5
   BNE   .switch@2_break
.case@3:
   LDA   g2
   LDA   g2+1
   LDA   g2+2
   LDA   g2+3
.case@4:
.case@5:
.switch@2_break:
   LDA   #$01
   STA   __local_f13__i17
.f13@rot@0:
   JSR   fin2@run8
   STA   __local_f13__2
   LDA   __zpabi_mix__v_0
   BNE   .carry_inc@1
   DEC   __local_f13__2
.carry_inc@1:
   DEC   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   STA   __local_f13__1
   LDA   __local_f13__2
   AND   #$01
   STA   __zpabi_mix__v_0
   LDA   __local_f13__1
   STA   gs7+1
   LDA   __zpabi_mix__v_0
   EOR   #$01
   SEC
   SBC   #$01
   STA   gs7+2
   LDA   __local_f13__l16
   SEC
   SBC   #$FF
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   JSR   fin0@run3
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   DEC   __local_f13__i17
   BNE   .f13@rot@0
   LDA   __local_f13__p14
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f13@run4:
   SUBROUTINE

   AND   #$7F
   EOR   #$40
   SEC
   SBC   #$40
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l18
   STA   __local_main__0
   INC   z5
   LDA   gs7+3
   AND   #$05
   JSR   f13@run4
   STA   __local_main__l18
   STA   gs7+3
   BNE   .lb_skip@0
   JMP   .if_else@5
.lb_skip@0:
   CLC
   ADC   __local_main__l18
   JSR   f13@run4
   BEQ   .if_end@6
   LDA   gs7+2
   AND   #$01
   LDY   #$FF
   STY   __local_main__2
   EOR   #$01
   SEC
   SBC   #$01
   STA   __local_main__1
   STA   gs7+2
   STY   gs7+1
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   INY
   LDA   (DPTR),Y
   STA   __local_main__0
   INY
   LDA   (DPTR),Y
   STA   __local_main__l18
   LDA   __local_main__2
   SEC
   SBC   __local_main__0
   STA   __local_main__0
   LDA   __local_main__1
   SBC   __local_main__l18
   STA   __local_main__l18
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   JSR   fin0@run3
   LDA   __local_main__0
   STA   __zpabi_mix__v_0
   LDA   __local_main__l18
   JSR   fin0@run6
.if_end@6:
   LDA   gs7+1
   SEC
   SBC   g1
   JSR   f13
   JSR   main@run2
   LDA   z5
   ORA   #$81
   STA   __local_main__l18
   STA   z5
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   JSR   fin1@run7
   LDA   __local_main__l18
   STA   __zpabi_mix__v_0
   JSR   mix
   JMP   .if_end@4
.if_else@5:
   LDA   #$00
   EOR   __local_main__l18
   JSR   f13
   JSR   main@run2
.if_end@4:
   JSR   fin0@run1
   INC   z5
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run2:
   SUBROUTINE

   LDA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   HARGS+11
   STA   __zpabi_mix__v_3
   JMP   mix

acc:
   DS.B  2

g2:
   DC.L  $0000007F

ga6:
   DC.B  $FF, $7F, $01, $FF

gs7:
   DS.B  1
   DC.W  $007F
   DC.B  $FF

gp8:
   DC.W  g1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g3
   STA   g3+1
   LDA   #$4E
   STA   g1
   LDA   #$AA
   STA   g1+1
   LDA   #$01
   STA   g4
   LDA   #$80
   STA   z5
   RTS
