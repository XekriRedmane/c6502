SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g3	EQU	$27
__local_main__l15	EQU	$80
__zpabi_f8__p9_0	EQU	$80
__local_main__0	EQU	$81
__zpabi_f8__p9_1	EQU	$81
__zpabi_f8__p9_2	EQU	$82
__zpabi_f8__p9_3	EQU	$83
__zpabi_f8__p9_4	EQU	$84
__zpabi_f8__p9_5	EQU	$85
__zpabi_f8__p9_6	EQU	$86
__zpabi_f8__p9_7	EQU	$87
__zpabi_f8__p10	EQU	$88
__zpabi_f8__pp11_0	EQU	$89
__zpabi_f8__pp11_1	EQU	$8A
__zpabi_mix__v_0	EQU	$8B
__zpabi_mix__v_1	EQU	$8C
__zpabi_mix__v_2	EQU	$8D
__zpabi_mix__v_3	EQU	$8E
__local_mix__0	EQU	$93
__local_mix__1	EQU	$94
__local_mix__2	EQU	$95
__local_mix__3	EQU	$96

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p9_2,__zpabi_f8__p9_3,__zpabi_f8__p9_4,__zpabi_f8__p9_5,__zpabi_f8__p9_6,__zpabi_f8__p9_7,__zpabi_f8__p10,__zpabi_f8__pp11_0,__zpabi_f8__pp11_1 locals=__local_f8__0,__local_f8__1,__local_f8__p9_1,__local_f8__p9_0 indirect=false in_cycle=false changes=__zpabi_f8__p9_0,__zpabi_f8__p9_1
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__l15,__local_main__0 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item g4 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$02 weight=22
; zp g3 size=1 at=auto def=true init=$7F weight=6
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
   JSR   fin0@run1
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run3
   JSR   mix
   LDA   g3
   JSR   fin0@run1
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   JMP   fin0@tail0

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin1@run0
   LDA   ga5
   JSR   fin1@run2
   LDA   ga5+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin1@run0
   LDA   ga5+1
   JSR   fin1@run2
   LDA   ga5+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run0
   LDA   ga5+2
   JSR   fin1@run2
   LDA   ga5+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run0
   LDA   ga5+3
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   gs6+1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin1@run0
   LDA   gs6+1
   JSR   fin1@run2
   LDA   gs6+2
   STA   __zpabi_mix__v_0
   LDA   gs6+3
   JMP   fin0@tail0

f8:
   SUBROUTINE

   LDA   #$02
   EOR   g4
   STA   __zpabi_mix__v_0
   LDA   #$00
   EOR   g4+1
   ORA   __zpabi_mix__v_0
   BEQ   .if_end@0
   LDA   __zpabi_f8__p10
   BMI   .thread@0
   LDA   #$00
   BEQ   .vthread@0
.thread@0:
   LDA   #$00
.vthread@0:
   STA   ga5+3
.if_end@0:
   LDA   #$FF
   STA   __zpabi_mix__v_0
   JSR   fin1@run0
   JSR   mix
   LDA   g3
   STA   __zpabi_mix__v_0
   CLC
   ADC   #$01
   AND   #$7F
   STA   g3
   LDA   __zpabi_mix__v_0
   AND   #$07
   CMP   #$03
   BEQ   .case@1
   CMP   #$04
   BEQ   .case@2
   CMP   #$05
   BNE   .lb_skip@0
   JMP   .switch@0_break
.lb_skip@0:
   CMP   #$07
   BEQ   .case@4
   JMP   .default@5
.case@1:
.case@2:
   LDY   #$00
   LDA   (__zpabi_f8__pp11_0),Y
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __zpabi_mix__v_1
   LDA   #$FF
   SEC
   SBC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   #$FF
   SBC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   SEC
   LDA   #$00
   SBC   g1
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   ORA   __zpabi_mix__v_0
   BEQ   .if_else@2
   LDA   g1
   LSR
   LSR
   LSR
   LSR
   LSR
   TAX
   LDA   g1
   ASL
   ASL
   ASL
   RTS
.if_else@2:
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   SEC
   SBC   #$01
   AND   #$7F
   STA   (DPTR),Y
   JMP   .switch@0_break
.case@4:
   LDA   g3
   STA   __zpabi_f8__p9_1
   LDY   #$00
   LDA   (__zpabi_f8__pp11_0),Y
   CLC
   ADC   #$05
   STA   __zpabi_mix__v_0
   STA   (__zpabi_f8__pp11_0),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __zpabi_mix__v_1
   LDA   __zpabi_f8__p9_1
   EOR   __zpabi_mix__v_0
   STA   __zpabi_f8__p9_0
   LDA   #$00
   EOR   __zpabi_mix__v_1
   STA   __zpabi_f8__p9_1
   BPL   .switch@0_break
   BMI   .switch@0_break
.default@5:
   LDA   #$9F
   LDY   #$00
   STA   (__zpabi_f8__pp11_0),Y
.switch@0_break:
   LDA   g2
   BNE   .carry_inc@0
   DEC   g2+1
.carry_inc@0:
   DEC   g2
   LDA   g2
   AND   #$07
   CMP   #$03
   BEQ   .switch@6_break
   CMP   #$04
   BEQ   .switch@6_break
   CMP   #$05
   BEQ   .switch@6_break
   CMP   #$06
.switch@6_break:
   LDA   __zpabi_f8__p9_0
   ORA   #$02
   LDX   __zpabi_f8__p9_1
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __local_main__l15
   LDA   #$00
   STA   __local_main__0
   DEC   g1
   INC   gs6+1
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
   DC.W  $0080

g4:
   DC.W  $007F

ga5:
   DC.B  $00, $03, $00, $FF

gs6:
   DS.B  1
   DC.B  $7F
   DC.W  $8000

gp7:
   DC.W  g3

__zp_init:
   SUBROUTINE

   LDA   #$02
   STA   g1
   LDA   #$7F
   STA   g3
   RTS
