SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$28
z7	EQU	$2C
g3	EQU	$2E
g1	EQU	$2F
g2	EQU	$31
__zpabi_f20__p22_0	EQU	$80
__zpabi_f20__p22_1	EQU	$81
__zpabi_f20__p23_0	EQU	$82
__zpabi_f20__p23_1	EQU	$83
__zpabi_mix__v_0	EQU	$84
__zpabi_mix__v_1	EQU	$85
__zpabi_mix__v_2	EQU	$86
__zpabi_mix__v_3	EQU	$87
__local_f20__0	EQU	$88
__local_main__w28	EQU	$88
__local_f20__1	EQU	$89
__local_f20__2	EQU	$8A
__local_main__1	EQU	$8A
__local_main__2	EQU	$8B
__local_f20__l24	EQU	$8C
__local_main__3	EQU	$8C
__local_main__l27_1	EQU	$8D
__local_main__l27_0	EQU	$8E
__local_f16__1	EQU	$90
__local_f16__2	EQU	$91
__local_f16__3	EQU	$92
__local_f16__4	EQU	$93
__local_f16__5	EQU	$94
__local_f10__w14	EQU	$97
__local_f10__0	EQU	$98
__local_f10__1	EQU	$99
__local_f10__2	EQU	$9A
__local_mix__0	EQU	$9C
__local_mix__1	EQU	$9D
__local_mix__2	EQU	$9E
__local_mix__3	EQU	$9F

; @zp-link-meta-begin
; def f10 params= locals=__local_f10__w14,__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3 indirect=false in_cycle=false
; def f16 params=__zpabi_f16__pp17_0,__zpabi_f16__pp17_1 locals=__local_f16__0,__local_f16__1,__local_f16__2,__local_f16__3,__local_f16__4,__local_f16__5,__local_f16__6,__local_f16__7 indirect=false in_cycle=false param_regs=A,X
; def f20 params=__zpabi_f20__p21,__zpabi_f20__p22_0,__zpabi_f20__p22_1,__zpabi_f20__p23_0,__zpabi_f20__p23_1 locals=__local_f20__0,__local_f20__1,__local_f20__2,__local_f20__3,__local_f20__l24,__local_f20__5,__local_f20__6 indirect=false in_cycle=false param_regs=A,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def fin3 params= locals=__local_fin3__0,__local_fin3__1 indirect=false in_cycle=false
; def main params= locals=__local_main__w28,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l27_1,__local_main__l27_0,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f16 -> f10
; call f16 -> mix
; call f20 -> f10
; call f20 -> f16
; call f20 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call fin3 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> fin3
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run1@run5 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run4 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin3 section=.text local=false zero=false
; item fin3@run3 section=.text local=true zero=false
; item f10 section=.text local=false zero=false
; item f16 section=.text local=false zero=false
; item f20 section=.text local=false zero=false
; item f20@run6 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g4 section=.data local=false zero=false
; item ga8 section=.data local=false zero=false
; item gs9 section=.data local=false zero=false
; item @1.s13 section=.bss local=true zero=true
; item @7.s19 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=2 at=auto def=true init=$00,$00
; zp z6 size=4 at=auto def=true init=$00,$00,$00,$80
; zp z7 size=2 at=auto def=true init=$0A,$00
; zp g3 size=1 at=auto def=true init=$01 weight=35
; zp g1 size=2 at=auto def=true init=$FF,$00 weight=34
; zp g2 size=1 at=auto def=true init=$80 weight=17
; runtime hargs_size=32 hargs_need=17
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
   JSR   fin0@run2
   LDA   g2
   JSR   fin0@run1
   LDA   g3
   JSR   fin0@run1
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   g4+2
   STA   __zpabi_mix__v_2
   LDA   g4+3
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin0@run0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run1@run5
   JMP   mix

fin0@run1@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run1@run5
   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin1@run4
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   LDA   z6+2
   STA   __zpabi_mix__v_2
   LDA   z6+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   z7+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run1@run5
   LDA   z7
   STA   __zpabi_mix__v_0
   LDA   z7+1
   JSR   fin1@run4
   LDA   ga8+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run1@run5
   LDA   ga8
   STA   __zpabi_mix__v_0
   LDA   ga8+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin1@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga8+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run1@run5
   LDA   ga8+2
   STA   __zpabi_mix__v_0
   LDA   ga8+3
   JSR   fin1@run4
   LDA   ga8+5
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run1@run5
   LDA   ga8+4
   STA   __zpabi_mix__v_0
   LDA   ga8+5
   JSR   fin1@run4
   LDA   ga8+7
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run1@run5
   LDA   ga8+6
   STA   __zpabi_mix__v_0
   LDA   ga8+7
   JSR   fin1@run4
   LDA   gs9+1
   STA   __zpabi_mix__v_0
   LDA   gs9+2
   STA   __zpabi_mix__v_1
   LDA   gs9+3
   STA   __zpabi_mix__v_2
   LDA   gs9+4
   JMP   fin0@tail0

fin3:
   SUBROUTINE

   JSR   fin3@run3
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin3@run3:
   SUBROUTINE

   LDA   gs9+9
   STA   __zpabi_mix__v_0
   LDA   gs9+10
   RTS

f10:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f10__w14
   STA   __local_f10__0
   LDA   z6
   STA   HARGS
   LDA   z6+1
   STA   HARGS+1
   LDA   z6+2
   STA   HARGS+2
   LDA   z6+3
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   STA   z6
   LDA   HARGS+6
   STA   z6+1
   LDA   HARGS+7
   STA   z6+2
   LDA   HARGS+8
   STA   z6+3
   LDA   g4
   STA   HARGS
   LDA   g4+1
   STA   HARGS+1
   LDA   g4+2
   STA   HARGS+2
   LDA   g4+3
   STA   HARGS+3
   LDA   g4+4
   STA   HARGS+4
   LDA   g4+5
   STA   HARGS+5
   LDA   g4+6
   STA   HARGS+6
   LDA   g4+7
   STA   HARGS+7
   LDA   #$05
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   STA   g4
   LDA   HARGS+11
   STA   g4+2
   LDA   HARGS+12
   STA   g4+3
   LDA   HARGS+13
   STA   g4+4
   LDA   HARGS+14
   STA   g4+5
   LDA   HARGS+15
   STA   g4+6
   LDA   HARGS+16
   STA   g4+7
   LDA   ga8+2
   STA   __local_f10__2
   LDA   ga8+3
   STA   __local_f10__1
   LDA   HARGS+10
   STA   g4+1
   LDA   HARGS+9
   EOR   __local_f10__2
   STA   ga8+4
   LDA   HARGS+10
   EOR   __local_f10__1
   STA   ga8+5
   LDA   @1.s13+1
   ORA   @1.s13
   BEQ   .cmp_end@12
   LDA   #$01
.cmp_end@12:
   AND   g3
   STA   z6
   LDA   #$00
   STA   z6+1
   STA   z6+2
   STA   z6+3
   STA   __local_f10__w14
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f10__2
   LDA   __local_f10__w14
   CMP   #$04
   BCS   .loop@0_break
   ASL   g3
   ASL   g3
   LDA   ga8+2
   LDX   ga8+3
   STX   __local_f10__1
   SEC
   SBC   #$01
   STA   __local_f10__0
   BCS   .carry_inc@0
   DEC   __local_f10__1
.carry_inc@0:
   STA   ga8+2
   LDA   __local_f10__1
   STA   ga8+3
   LDA   __local_f10__0
   EOR   g2
   STA   z6
   LDA   __local_f10__1
   STA   z6+1
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   z6+2
   STA   z6+3
   LDA   g3
   AND   #$03
   STA   g3
   STA   g1
   LDA   #$00
   STA   g1+1
   LDA   __local_f10__2
   LSR
   LSR
   LSR
   LSR
   JSR   fin0@run1
   LDA   __local_f10__2
   STA   __local_f10__w14
   JMP   .loop@0_continue
.loop@0_break:
   LDA   @1.s13
   ORA   #$05
   STA   @1.s13
   LDA   z6
   RTS

f16:
   SUBROUTINE

   JSR   f10
   LDA   z5+1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_f16__5
   STA   __local_f16__4
   STA   __local_f16__3
   STA   __local_f16__2
   STA   __local_f16__1
   STA   __zpabi_mix__v_0
   LDA   z5
   SEC
   SBC   g4
   LDA   z5+1
   SBC   g4+1
   LDA   __local_f16__5
   SBC   g4+2
   LDA   __local_f16__4
   SBC   g4+3
   LDA   __local_f16__3
   SBC   g4+4
   LDA   __local_f16__2
   SBC   g4+5
   LDA   __local_f16__1
   SBC   g4+6
   LDA   __zpabi_mix__v_0
   SBC   g4+7
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   JSR   fin0@run1@run5
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   INC   z6
   BNE   .inc_done@0
   INC   z6+1
   BNE   .inc_done@0
   INC   z6+2
   BNE   .inc_done@0
   INC   z6+3
.inc_done@0:
   JSR   f10
   LDA   @7.s19
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   TAX
   LDA   @7.s19
   RTS

f20:
   SUBROUTINE

   LDA   #$03
   STA   __local_f20__l24
   LDA   #$00
   STA   __local_f20__l24+1
   LDA   z6
   SEC
   SBC   #$01
   STA   z6
   LDA   z6+1
   SBC   #$00
   STA   z6+1
   LDA   z6+2
   SBC   #$00
   STA   z6+2
   BCS   .carry_inc@1
   DEC   z6+3
.carry_inc@1:
   LDA   ga8
   ORA   z6
   STA   z5
   LDA   ga8+1
   ORA   z6+1
   STA   z5+1
   LDX   #>__local_f20__l24
   LDA   #<__local_f20__l24
   JSR   f16
   JSR   f20@run6
   LDA   ga8+6
   STA   __local_f20__2
   LDA   __local_f20__l24
   STA   __zpabi_mix__v_0
   LDA   __local_f20__l24+1
   JSR   fin0@run2
   LDX   #>__local_f20__l24
   LDA   #<__local_f20__l24
   JSR   f16
   JSR   f20@run6
   LDA   g3
   AND   #$03
   BEQ   .if_else@1
   LDA   __local_f20__2
   SEC
   SBC   #$05
   RTS
.if_else@1:
   JSR   f10
   STA   __local_f20__l24
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f20__l24+1
   LDA   __zpabi_f20__p22_0
   CLC
   ADC   #$01
   STA   __local_f20__1
   LDA   __zpabi_f20__p22_1
   ADC   #$00
   STA   __local_f20__0
   LDA   #$00
   AND   __local_f20__1
   STA   __zpabi_mix__v_0
   LDA   #$00
   AND   __local_f20__0
   JSR   fin0@run2
   LDA   #$01
   RTS

f20@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JMP   fin0@run0

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__w28
   JSR   fin3@run3
   STA   __local_main__w28
   LDA   __zpabi_mix__v_0
   SEC
   SBC   #$01
   STA   gs9+9
   LDA   __local_main__w28
   SBC   #$00
   STA   gs9+10
   LDA   #$7F
   AND   __local_main__w28
   JSR   fin0@run2
   LDA   g4
   SEC
   SBC   #$02
   EOR   #$FF
   AND   #$07
   BNE   .lb_skip@2
   JMP   .switch@2_break
.lb_skip@2:
   CMP   #$04
   BEQ   .case@4
   CMP   #$06
   BNE   .lb_skip@1
   JMP   .switch@2_break
.lb_skip@1:
   CMP   #$07
   BEQ   .case@6
   JMP   .switch@2_break
.case@4:
   LDA   gs9+9
   STA   __local_main__2
   LDA   gs9+10
   STA   __local_main__1
   LDX   #$06
.shift_loop@23:
   ASL   z5
   ROL   z5+1
   DEX
   BNE   .shift_loop@23
   LDA   __local_main__2
   SEC
   SBC   z5
   STA   __zpabi_mix__v_0
   LDA   __local_main__1
   SBC   z5+1
   JSR   fin0@run2
   JMP   .switch@2_break
.case@6:
   LDA   ga8
   LDX   ga8+1
   STX   __zpabi_mix__v_0
   CLC
   ADC   #$81
   BCC   .carry_inc@2
   INC   __zpabi_mix__v_0
.carry_inc@2:
   STA   ga8
   STA   g1
   LDA   __zpabi_mix__v_0
   STA   ga8+1
   STA   g1+1
   LDA   #$FF
   STA   __local_main__l27_0
   STA   __local_main__l27_1
   LDA   #$00
   STA   __local_main__w28
.loop@7_continue:
   CLC
   ADC   #$01
   STA   __local_main__3
   LDA   __local_main__w28
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .loop@7_break
.lb_skip@0:
   LDA   g1
   EOR   #$81
   STA   g1
   LDA   g2
   AND   #$03
   ADC   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   ADC   #$00
   ORA   __zpabi_mix__v_0
   BEQ   .if_else@5
   LDA   __local_main__3
   JSR   fin0@run1
   LDA   z7
   BNE   .carry_inc@3
   DEC   z7+1
.carry_inc@3:
   DEC   z7
   JSR   fin3@run3
   STA   __local_main__w28
   LDA   z7
   ORA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   z7+1
   ORA   __local_main__w28
   JSR   fin0@run2
   JMP   .if_end@4
.if_else@5:
   LDA   __local_main__l27_0
   SEC
   SBC   #$03
   STA   __local_main__l27_0
   BCS   .carry_inc@4
   DEC   __local_main__l27_1
.carry_inc@4:
.if_end@4:
   ASL   z5
   ROL   z5+1
   ASL   z5
   ROL   z5+1
   ASL   z5
   ROL   z5+1
   LDA   z5
   BNE   .carry_inc@5
   DEC   z5+1
.carry_inc@5:
   DEC   z5
   LDA   __local_main__3
   STA   __local_main__w28
   JMP   .loop@7_continue
.loop@7_break:
.switch@2_break:
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

g4:
   DC.L  $0000FFFF
   DC.L  $00000000

ga8:
   DC.W  $0002
   DC.W  $8000
   DC.W  $0001
   DC.W  $0002

gs9:
   DS.B  1
   DC.L  $80000000
   DC.L  $00000000
   DS.B  2

@1.s13:
   DS.B  2

@7.s19:
   DC.B  $7F

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   z5
   STA   z5+1
   STA   z6
   STA   z6+1
   STA   z6+2
   STA   z7+1
   LDA   #$FF
   STA   g1
   LDA   #$80
   STA   g2
   STA   z6+3
   LDA   #$01
   STA   g3
   LDA   #$0A
   STA   z7
   RTS
