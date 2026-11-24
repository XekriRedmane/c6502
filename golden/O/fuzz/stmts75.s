SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$27
z7	EQU	$2F
g4	EQU	$33
gp10	EQU	$35
__zpabi_f19__p21_0	EQU	$80
__zpabi_f19__p21_1	EQU	$81
__zpabi_f19__pp22_0	EQU	$82
__zpabi_f19__pp22_1	EQU	$83
__zpabi_mix__v_0	EQU	$84
__zpabi_mix__v_1	EQU	$85
__zpabi_mix__v_2	EQU	$86
__zpabi_mix__v_3	EQU	$87
__local_main__0	EQU	$88
__local_f19__0	EQU	$89
__local_main__1	EQU	$89
__local_main__2	EQU	$8A
__local_f19__l25	EQU	$8B
__local_main__3	EQU	$8B
__local_main__4	EQU	$8C
__local_main__5	EQU	$8D
__local_f12__l13	EQU	$8E
__local_main__6	EQU	$8E
__local_f12__0	EQU	$8F
__local_main__7	EQU	$8F
__local_main__w33	EQU	$90
__local_f12__2	EQU	$91
__local_main__l30	EQU	$91
__local_f12__w18	EQU	$92
__local_f12__3	EQU	$93
__local_f12__4	EQU	$94
__local_f12__5	EQU	$95
__local_f12__w17	EQU	$96
__local_f12__l15	EQU	$97
__local_f12__6	EQU	$98
__local_mix__0	EQU	$9A
__local_mix__1	EQU	$9B
__local_mix__2	EQU	$9C
__local_mix__3	EQU	$9D

; @zp-link-meta-begin
; def f12 params= locals=__local_f12__l13,__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__w18,__local_f12__3,__local_f12__4,__local_f12__5,__local_f12__w17,__local_f12__l15,__local_f12__6,__local_f12__7 indirect=false in_cycle=false
; def f19 params=__zpabi_f19__p20,__zpabi_f19__p21_0,__zpabi_f19__p21_1,__zpabi_f19__pp22_0,__zpabi_f19__pp22_1 locals=__local_f19__w27,__local_f19__0,__local_f19__1,__local_f19__l25,__local_f19__2,__local_f19__3 indirect=false in_cycle=false param_regs=A,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def fin3 params= locals=__local_fin3__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__w33,__local_main__l30 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f12 -> mix
; call f19 -> f12
; call f19 -> mix
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
; item fin0@run6 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run5 section=.text local=true zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin3 section=.text local=false zero=false
; item f12 section=.text local=false zero=false
; item f12@run4 section=.text local=true zero=false
; item f12@run1 section=.text local=true zero=false
; item f12@run0 section=.text local=true zero=false
; item f19 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item g3 section=.bss local=false zero=true
; item ga8 section=.data local=false zero=false
; item gs9 section=.data local=false zero=false
; item gp11 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=1 at=auto def=true init=$00
; zp z6 size=8 at=auto def=true init=$80,$00,$00,$00,$00,$00,$00,$00
; zp z7 size=4 at=auto def=true init=$00,$01,$00,$00
; zp g4 size=2 at=auto def=true init=$00,$80 weight=9
; zp gp10 size=2 at=auto def=true init=<g1,>g1 weight=8
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
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JSR   fin0@run2
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
   STA   __zpabi_mix__v_1
   JSR   fin0@run6
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run6:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run2

fin0@run2:
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
   JSR   fin1@run3
   LDA   z5
   JSR   fin1@run5
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   LDA   z6+2
   STA   __zpabi_mix__v_2
   LDA   z6+3
   STA   __zpabi_mix__v_3
   LDA   z6+4
   LDA   z6+5
   LDA   z6+6
   LDA   z6+7
   JSR   mix
   LDA   z7
   STA   __zpabi_mix__v_0
   LDA   z7+1
   STA   __zpabi_mix__v_1
   LDA   z7+2
   STA   __zpabi_mix__v_2
   LDA   z7+3
   JSR   fin0@run2
   LDA   ga8
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin1@run3
   LDA   ga8
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   ga8+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run3
   LDA   ga8+1
   JSR   fin1@run5
   LDA   ga8+2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run3
   LDA   ga8+2
   JSR   fin1@run5
   LDA   ga8+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin1@run3
   LDA   ga8+3
   JSR   fin1@run5
   LDA   gs9+1
   STA   __zpabi_mix__v_0
   LDA   gs9+2
   STA   __zpabi_mix__v_1
   LDA   gs9+3
   STA   __zpabi_mix__v_2
   LDA   gs9+4
   STA   __zpabi_mix__v_3
   JMP   mix

fin3:
   SUBROUTINE

   LDA   gs9+5
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin1@run3
   LDA   gs9+5
   STA   __zpabi_mix__v_0
   JMP   mix

f12:
   SUBROUTINE

   LDA   #$7F
   STA   __local_f12__l13
   LDA   #$00
   STA   __local_f12__0
   STA   __local_f12__l15
   LDA   #$01
   STA   __local_f12__6
   LDA   __local_f12__l15
   STA   __local_f12__2
   LDA   __local_f12__6
   STA   __zpabi_mix__v_1
   LDA   __local_f12__2
   SEC
   SBC   #$01
   STA   __local_f12__0
   LDA   __zpabi_mix__v_1
   SBC   #$00
   LDX   __local_f12__0
   STX   __local_f12__l15
   STA   __local_f12__6
   LDA   __local_f12__2
   EOR   #$FF
   STA   __zpabi_mix__v_0
   JSR   f12@run0
   LDA   #$02
   STA   HARGS+8
   JSR   asr64
   JSR   f12@run1
   LDA   HARGS+9
   STA   __zpabi_mix__v_0
   LDA   HARGS+10
   STA   __zpabi_mix__v_1
   LDA   HARGS+11
   STA   __zpabi_mix__v_2
   LDA   HARGS+12
   JSR   fin0@run2
   LDA   #$00
   STA   __local_f12__w17
.loop@0_start:
   LDA   z6
   LDX   z6+1
   STX   __local_f12__5
   LDX   z6+2
   STX   __local_f12__4
   LDX   z6+3
   STX   __local_f12__3
   LDX   z6+4
   STX   __local_f12__w18
   LDX   z6+5
   STX   __local_f12__2
   LDX   z6+6
   STX   __zpabi_mix__v_1
   LDX   z6+7
   STX   __local_f12__0
   SEC
   SBC   #$03
   STA   __local_f12__l13
   LDA   __local_f12__5
   SBC   #$00
   STA   __local_f12__5
   LDA   __local_f12__4
   SBC   #$00
   STA   __local_f12__4
   LDA   __local_f12__3
   SBC   #$00
   STA   __local_f12__3
   LDA   __local_f12__w18
   SBC   #$00
   STA   __local_f12__w18
   LDA   __local_f12__2
   SBC   #$00
   STA   __local_f12__2
   LDA   __zpabi_mix__v_1
   SBC   #$00
   STA   __zpabi_mix__v_1
   BCS   .carry_inc@0
   DEC   __local_f12__0
.carry_inc@0:
   LDA   __local_f12__l13
   STA   z6
   LDA   __local_f12__5
   STA   z6+1
   LDA   __local_f12__4
   STA   z6+2
   LDA   __local_f12__3
   STA   z6+3
   LDA   __local_f12__w18
   STA   z6+4
   LDA   __local_f12__2
   STA   z6+5
   LDA   __zpabi_mix__v_1
   STA   z6+6
   LDA   __local_f12__0
   STA   z6+7
   INC   __local_f12__w17
   BNE   .lb_skip@1
   JMP   .loop@0_start
.lb_skip@1:
   LDA   ga8+3
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f12__4
   STA   __local_f12__3
   STA   __local_f12__w18
   JSR   f12@run4
   STA   __local_f12__2
   LDY   #$01
   LDA   (DPTR),Y
   STA   __zpabi_mix__v_1
   INY
   LDA   (DPTR),Y
   STA   __local_f12__0
   INY
   LDA   (DPTR),Y
   STA   __local_f12__l13
   LDA   ga8+3
   CLC
   ADC   __local_f12__2
   STA   __local_f12__5
   LDA   __local_f12__4
   ADC   __zpabi_mix__v_1
   STA   __local_f12__4
   LDA   __local_f12__3
   ADC   __local_f12__0
   STA   __local_f12__3
   LDA   __local_f12__w18
   ADC   __local_f12__l13
   STA   __local_f12__w18
   LDA   z5
   CLC
   ADC   #$01
   STA   __local_f12__2
   STA   z5
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __zpabi_mix__v_1
   STA   __local_f12__0
   STA   __local_f12__l13
   LDA   __local_f12__5
   EOR   __local_f12__2
   STA   __zpabi_mix__v_0
   LDA   __local_f12__4
   EOR   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   __local_f12__3
   EOR   __local_f12__0
   STA   __zpabi_mix__v_2
   LDA   __local_f12__w18
   EOR   __local_f12__l13
   JSR   fin0@run2
   LDA   #$00
   STA   __local_f12__w18
.loop@1_start:
   LDA   __local_f12__l15
   ORA   __local_f12__6
   BEQ   .and_false@0
   LDY   #$01
   LDA   (gp10),Y
   STA   __zpabi_mix__v_1
   INY
   LDA   (gp10),Y
   STA   __local_f12__0
   INY
   LDA   (gp10),Y
   STA   __local_f12__l13
   LDY   #$00
   LDA   (gp10),Y
   ORA   __zpabi_mix__v_1
   ORA   __local_f12__0
   ORA   __local_f12__l13
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f12__0
   LDA   #$00
   BEQ   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f12__0
.and_end@1:
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f12__0
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs9+5
   STA   __local_f12__0
   LDX   #$03
.shift_loop@18:
   LDA   __local_f12__0
   ASL
   ROR   __local_f12__0
   DEX
   BNE   .shift_loop@18
   LDA   __local_f12__0
   STA   gs9+5
   STA   __local_f12__l13
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   JSR   fin1@run3
   LDA   __local_f12__l13
   JSR   fin1@run5
   LDA   gs9+5
   STA   __local_f12__2
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_mix__v_1
   STA   __local_f12__0
   STA   __local_f12__l13
   LDA   __local_f12__2
   SEC
   SBC   g1
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   SBC   g1+1
   STA   __zpabi_mix__v_1
   LDA   __local_f12__0
   SBC   g1+2
   STA   __zpabi_mix__v_2
   LDA   __local_f12__l13
   SBC   g1+3
   JSR   fin0@run2
   INC   __local_f12__w18
   LDA   __local_f12__w18
   CMP   #$03
   BCS   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   LDA   z5
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_f12__4
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_f12__3
   STA   __local_f12__w18
   STA   __local_f12__2
   STA   __zpabi_mix__v_1
   STA   __local_f12__0
   STA   __local_f12__l13
   LDA   __local_f12__4
   SBC   #$01
   LDA   __local_f12__3
   SBC   #$00
   LDA   __local_f12__w18
   SBC   #$00
   LDA   __local_f12__2
   SBC   #$00
   LDA   __zpabi_mix__v_1
   SBC   #$00
   LDA   __local_f12__0
   SBC   #$00
   LDA   __local_f12__l13
   SBC   #$00
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   STA   __local_f12__l13
   LDA   #$00
   BPL   .sx_done@30
   LDA   #$FF
.sx_done@30:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f12__l13
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f12@run4:
   SUBROUTINE

   LDA   gp11
   STA   DPTR
   LDA   gp11+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

f12@run1:
   SUBROUTINE

   LDA   HARGS+9
   STA   z6
   LDA   HARGS+10
   STA   z6+1
   LDA   HARGS+11
   STA   z6+2
   LDA   HARGS+12
   STA   z6+3
   LDA   HARGS+13
   STA   z6+4
   LDA   HARGS+14
   STA   z6+5
   LDA   HARGS+15
   STA   z6+6
   LDA   HARGS+16
   STA   z6+7
   RTS

f12@run0:
   SUBROUTINE

   JSR   fin0@run6
   LDA   z6
   STA   HARGS
   LDA   z6+1
   STA   HARGS+1
   LDA   z6+2
   STA   HARGS+2
   LDA   z6+3
   STA   HARGS+3
   LDA   z6+4
   STA   HARGS+4
   LDA   z6+5
   STA   HARGS+5
   LDA   z6+6
   STA   HARGS+6
   LDA   z6+7
   STA   HARGS+7
   RTS

f19:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f19__l25
   JSR   f12@run4
   STA   __zpabi_mix__v_0
   LDY   #$01
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   INY
   LDA   (DPTR),Y
   LDA   #$80
   ORA   __zpabi_mix__v_0
   AND   #$07
   BEQ   .case@3
   CMP   #$02
   BNE   .lb_skip@4
   JMP   .switch@2_break
.lb_skip@4:
   CMP   #$05
   BNE   .lb_skip@3
   JMP   .case@8
.lb_skip@3:
   CMP   #$07
   BNE   .lb_skip@2
   JMP   .case@9
.lb_skip@2:
   JMP   .default@10
.case@3:
   LDA   #$00
   STA   __zpabi_mix__v_0
   LDA   #$FD
   STA   __zpabi_mix__v_1
   LDA   #$FF
   STA   __zpabi_mix__v_2
   JSR   fin0@run2
   LDA   #$00
   STA   __zpabi_mix__v_0
.loop@4_continue:
   CLC
   ADC   #$01
   STA   __local_f19__0
   LDA   __zpabi_mix__v_0
   BNE   .switch@2_break
   LDA   z7
   STA   HARGS
   LDA   z7+1
   STA   HARGS+1
   LDA   z7+2
   STA   HARGS+2
   LDA   z7+3
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   z7
   LDA   HARGS+6
   STA   z7+1
   LDA   HARGS+7
   STA   z7+2
   LDA   HARGS+8
   STA   z7+3
   JSR   f12
   LDA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   HARGS+11
   JSR   fin0@run2
   LDA   __local_f19__l25
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run2
   LDA   __local_f19__0
   STA   __zpabi_mix__v_0
   JMP   .loop@4_continue
.case@8:
.case@9:
.default@10:
.switch@2_break:
   LDA   gs9+1
   SEC
   SBC   g1
   LDA   gs9+2
   SBC   g1+1
   LDA   gs9+3
   SBC   g1+2
   LDA   gs9+4
   SBC   g1+3
   BVC   .cmp_novf@33
   EOR   #$80
.cmp_novf@33:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l30
   STA   __local_main__w33
.loop@11_start:
   LDA   z6
   LDX   z6+1
   STX   __local_main__6
   LDX   z6+2
   STX   __local_main__5
   LDX   z6+3
   STX   __local_main__4
   LDX   z6+4
   STX   __local_main__3
   LDX   z6+5
   STX   __local_main__2
   LDX   z6+6
   STX   __local_main__1
   LDX   z6+7
   STX   __local_main__0
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .cond_else@2
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   JMP   .cond_end@3
.cond_else@2:
   LDA   #$00
   SEC
   SBC   __local_main__w33
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
.cond_end@3:
   STA   __zpabi_mix__v_1
   JSR   f12@run0
   LDA   #$06
   STA   HARGS+8
   JSR   asl64
   JSR   f12@run1
   LDA   #$FF
   CLC
   ADC   HARGS+9
   STA   __local_main__7
   LDA   #$FF
   ADC   HARGS+10
   STA   __local_main__6
   LDA   #$FF
   ADC   HARGS+11
   STA   __local_main__5
   LDA   #$7F
   ADC   HARGS+12
   STA   __local_main__4
   LDA   #$00
   ADC   HARGS+13
   STA   __local_main__3
   LDA   #$00
   ADC   HARGS+14
   STA   __local_main__2
   LDA   #$00
   ADC   HARGS+15
   STA   __local_main__1
   LDA   #$00
   ADC   HARGS+16
   STA   __local_main__0
   LDA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@4
   LDA   gs9+5
   STA   __local_main__1
   ASL
   CLC
   ADC   __local_main__1
   STA   gs9+5
   JMP   .loop@11_continue
.if_end@4:
   DEC   __local_main__l30
.loop@11_continue:
   INC   __local_main__w33
   LDA   __local_main__w33
   CMP   #$02
   BCS   .lb_skip@5
   JMP   .loop@11_start
.lb_skip@5:
   LDA   z7
   CLC
   ADC   #$05
   STA   z7
   BCC   .carry_inc@1
   INC   z7+1
   BNE   .carry_inc@1
   INC   z7+2
   BNE   .carry_inc@1
   INC   z7+3
.carry_inc@1:
   AND   #$AA
   STA   g1
   LDA   z7+1
   AND   #$AA
   STA   g1+1
   LDA   z7+2
   AND   #$AA
   STA   g1+2
   LDA   z7+3
   AND   #$2A
   STA   g1+3
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

g1:
   DS.B  4

g2:
   DC.L  $00000001

g3:
   DS.B  2

ga8:
   DC.B  $01, $00, $00, $00

gs9:
   DS.B  1
   DC.L  $00008000
   DS.B  1

gp11:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g4
   STA   z5
   STA   z6+1
   STA   z6+2
   STA   z6+3
   STA   z6+4
   STA   z6+5
   STA   z6+6
   STA   z6+7
   STA   z7
   STA   z7+2
   STA   z7+3
   LDA   #$80
   STA   g4+1
   STA   z6
   LDA   #$01
   STA   z7+1
   LDA   #<g1
   STA   gp10
   LDA   #>g1
   STA   gp10+1
   RTS
