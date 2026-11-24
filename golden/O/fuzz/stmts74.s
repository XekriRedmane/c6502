SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g2	EQU	$27
g3	EQU	$28
g4	EQU	$29
__local_f20__l22	EQU	$80
__zpabi_f12__p15_0	EQU	$80
__zpabi_mix__v_0	EQU	$80
__local_f20__0	EQU	$81
__zpabi_f12__p15_1	EQU	$81
__zpabi_mix__v_1	EQU	$81
__local_f20__1	EQU	$82
__zpabi_f12__p15_2	EQU	$82
__zpabi_mix__v_2	EQU	$82
__local_f20__2	EQU	$83
__zpabi_f12__p15_3	EQU	$83
__zpabi_mix__v_3	EQU	$83
__zpabi_f12__p15_4	EQU	$84
__zpabi_f12__p15_5	EQU	$85
__zpabi_f12__p15_6	EQU	$86
__zpabi_f12__p15_7	EQU	$87
__local_f8__4	EQU	$88
__local_main__w27	EQU	$88
__local_main__1	EQU	$8A
__local_main__2	EQU	$8B
__local_f12__0	EQU	$8E
__local_mix__0	EQU	$8E
__local_f12__1	EQU	$8F
__local_mix__1	EQU	$8F
__local_mix__2	EQU	$90
__local_mix__3	EQU	$91

; @zp-link-meta-begin
; def f12 params=__zpabi_f12__p13,__zpabi_f12__p14,__zpabi_f12__p15_0,__zpabi_f12__p15_1,__zpabi_f12__p15_2,__zpabi_f12__p15_3,__zpabi_f12__p15_4,__zpabi_f12__p15_5,__zpabi_f12__p15_6,__zpabi_f12__p15_7 locals=__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__3 indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; def f20 params=__zpabi_f20__pp21_0,__zpabi_f20__pp21_1 locals=__local_f20__l22,__local_f20__0,__local_f20__1,__local_f20__2 indirect=false in_cycle=false param_regs=A,X
; def f8 params=__zpabi_f8__pp9_0,__zpabi_f8__pp9_1 locals=__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__3,__local_f8__4 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__w27,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f12
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f12 section=.text local=false zero=false
; item f20 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$FF weight=10
; zp g2 size=1 at=auto def=true init=$80 weight=6
; zp g3 size=1 at=auto def=true init=$00 weight=4
; zp g4 size=2 at=auto def=true init=$7F,$00 weight=7
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

   LDA   g1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   JSR   fin0@run1
   JSR   fin0@run0
   LDA   g3
   JSR   fin0@run1
   JSR   fin0@run0
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   STA   __zpabi_mix__v_1
   LDA   ga5+2
   STA   __zpabi_mix__v_2
   LDA   ga5+3
   JSR   fin0@run0
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   STA   __zpabi_mix__v_1
   LDA   ga5+6
   STA   __zpabi_mix__v_2
   LDA   ga5+7
   JSR   fin0@run0
   LDA   ga5+8
   STA   __zpabi_mix__v_0
   LDA   ga5+9
   STA   __zpabi_mix__v_1
   LDA   ga5+10
   STA   __zpabi_mix__v_2
   LDA   ga5+11
   JSR   fin0@run0
   LDA   ga5+12
   STA   __zpabi_mix__v_0
   LDA   ga5+13
   STA   __zpabi_mix__v_1
   LDA   ga5+14
   STA   __zpabi_mix__v_2
   LDA   ga5+15
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   JSR   fin0@run0
   LDA   gs6+5
   JSR   fin0@run1
   JMP   fin0@tail0

f8:
   SUBROUTINE

   LDA   g2
   SEC
   SBC   #$03
   STA   __local_f8__4
   STA   g2
   LDA   #$80
   SEC
   SBC   ga5+8
   LDA   #$00
   SBC   ga5+9
   LDA   #$00
   SBC   ga5+10
   LDA   #$00
   SBC   ga5+11
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_0
   LDA   __local_f8__4
   EOR   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g3
   ORA   ga5+4
   STA   g4
   LDA   ga5+5
   STA   g4+1
   LDA   gs6+1
   SEC
   SBC   #$01
   STA   gs6+1
   LDA   gs6+2
   SBC   #$00
   STA   gs6+2
   LDA   gs6+3
   SBC   #$00
   STA   gs6+3
   BCS   .carry_inc@0
   DEC   gs6+4
.carry_inc@0:
   LDA   gs6+5
   STA   __zpabi_mix__v_0
   CMP   __zpabi_mix__v_0
   BEQ   .cmp_true@5
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDX   #$00
   LDA   #$80
   RTS

f12:
   SUBROUTINE

   LDA   #$FF
   STA   g4
   LDA   #$7F
   STA   g4+1
   LDA   ga5+4
   EOR   #$01
   STA   ga5+4
   LDA   g1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_f12__1
   LDA   g1
   SEC
   SBC   #$FF
   STA   __local_f12__0
   LDA   __local_f12__1
   SBC   #$7F
   TAX
   LDA   __local_f12__0
   RTS

f20:
   SUBROUTINE

   LDA   #$07
   STA   __local_f20__l22
   LDA   #$00
   STA   __local_f20__0
   STA   __local_f20__1
   STA   __local_f20__2
   LDA   g2
   STA   g4
   LDA   #$00
   STA   g4+1
   LDA   gs6+1
   STA   g3
   LDA   g2
   ORA   #$05
   STA   g2
   LDA   #$80
   STA   HARGS+8
   LDA   #$FF
   STA   HARGS+9
   LDY   #$FF
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   gs6+1
   STA   __local_main__2
   LDA   gs6+2
   STA   __local_main__1
   LDA   gs6+3
   STA   __zpabi_mix__v_0
   LDA   gs6+4
   STA   __local_main__w27
   LDA   __local_main__2
   CLC
   ADC   #$01
   STA   gs6+1
   LDA   __local_main__1
   ADC   #$00
   STA   gs6+2
   LDA   __zpabi_mix__v_0
   ADC   #$00
   STA   gs6+3
   LDA   __local_main__w27
   ADC   #$00
   STA   gs6+4
   LDA   __local_main__2
   STA   g3
   LDA   #$00
   STA   __local_main__w27
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_main__2
   LDA   __local_main__w27
   CMP   #$02
   BCS   .loop@0_break
   LDA   __local_main__2
   AND   #$03
   ASL
   ASL
   TAX
   LDA   ga5,X
   STA   __local_main__1
   LDA   g1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   HARGS+1
   LDA   g1
   STA   HARGS
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __zpabi_f12__p15_2
   STA   __zpabi_f12__p15_3
   STA   __zpabi_f12__p15_4
   STA   __zpabi_f12__p15_5
   STA   __zpabi_f12__p15_6
   STA   __zpabi_f12__p15_7
   LDA   HARGS+6
   STA   __zpabi_f12__p15_0
   LDA   HARGS+7
   STA   __zpabi_f12__p15_1
   LDX   __local_main__1
   LDA   #$00
   JSR   f12
   STA   __zpabi_mix__v_0
   TXA
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   STX   __zpabi_mix__v_1
   JSR   mix
   LDA   __local_main__2
   STA   __local_main__w27
   JMP   .loop@0_continue
.loop@0_break:
   LDA   g4
   BNE   .carry_inc@1
   DEC   g4+1
.carry_inc@1:
   DEC   g4
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDA   #$FF
   STA   (DPTR),Y
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

ga5:
   DC.L  $7FFFFFFF
   DC.L  $0000FFFF
   DC.L  $80000000
   DC.L  $000000FF

gs6:
   DS.B  1
   DC.L  $00000080
   DS.B  1

gp7:
   DC.W  g3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g3
   STA   g4+1
   LDA   #$FF
   STA   g1
   LDA   #$80
   STA   g2
   LDA   #$7F
   STA   g4
   RTS
