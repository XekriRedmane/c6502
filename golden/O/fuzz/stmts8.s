SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
g3	EQU	$28
g4	EQU	$29
g1	EQU	$2A
g2	EQU	$2B
gp9	EQU	$2C
__zpabi_f10__p11_0	EQU	$80
__zpabi_f22__p24_0	EQU	$80
__zpabi_f10__p11_1	EQU	$81
__zpabi_f22__p24_1	EQU	$81
__zpabi_f10__p11_2	EQU	$82
__zpabi_f22__p24_2	EQU	$82
__zpabi_f10__p11_3	EQU	$83
__zpabi_f22__p24_3	EQU	$83
__zpabi_f10__p12_0	EQU	$84
__zpabi_f22__pp25_0	EQU	$84
__zpabi_f10__p12_1	EQU	$85
__zpabi_f22__pp25_1	EQU	$85
__zpabi_mix__v_0	EQU	$86
__zpabi_mix__v_1	EQU	$87
__zpabi_mix__v_2	EQU	$88
__zpabi_mix__v_3	EQU	$89
__local_main__0	EQU	$8A
__local_main__1	EQU	$8B
__local_f10__0	EQU	$8C
__local_main__2	EQU	$8C
__local_main__l31	EQU	$8E
__local_main__l30	EQU	$8F
__local_f17__2	EQU	$92
__local_f22__p23	EQU	$92
__local_f17__3	EQU	$93
__local_f17__pp18	EQU	$94
__local_f17__4	EQU	$95
__local_f17__7	EQU	$98
__local_mix__0	EQU	$9A
__local_mix__1	EQU	$9B
__local_mix__2	EQU	$9C
__local_mix__3	EQU	$9D

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1,__zpabi_f10__p11_2,__zpabi_f10__p11_3,__zpabi_f10__p12_0,__zpabi_f10__p12_1 locals=__local_f10__w14,__local_f10__w15,__local_f10__0 indirect=false in_cycle=false changes=__zpabi_f10__p12_1
; def f17 params=__zpabi_f17__pp18_0,__zpabi_f17__pp18_1 locals=__local_f17__0,__local_f17__1,__local_f17__2,__local_f17__3,__local_f17__pp18,__local_f17__4,__local_f17__5,__local_f17__6,__local_f17__7,__local_f17__8 indirect=false in_cycle=false param_regs=A,X
; def f22 params=__zpabi_f22__p23,__zpabi_f22__p24_0,__zpabi_f22__p24_1,__zpabi_f22__p24_2,__zpabi_f22__p24_3,__zpabi_f22__pp25_0,__zpabi_f22__pp25_1 locals=__local_f22__0,__local_f22__1,__local_f22__p23 indirect=false in_cycle=false param_regs=A,,,,,,
; def fin0 params= locals=__local_fin0__0 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l31,__local_main__l30 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f17 -> mix
; call f22 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f17
; call main -> f22
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run2 section=.text local=true zero=false
; item f10 section=.text local=false zero=false
; item f17 section=.text local=false zero=false
; item f22 section=.text local=false zero=false
; item f22@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=2 at=auto def=true init=$FF,$FF
; zp g3 size=1 at=auto def=true init=$80 weight=195
; zp g4 size=1 at=auto def=true init=$02 weight=85
; zp g1 size=1 at=auto def=true init=$FF weight=23
; zp g2 size=1 at=auto def=true init=$00 weight=7
; zp gp9 size=2 at=auto def=true init=<g2,>g2 weight=13
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
   JSR   fin0@run4
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g3
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   JSR   fin0@run3
   LDA   g4
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g4
   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   fin0@run3

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   JMP   fin0@run1

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   STA   __zpabi_mix__v_1
   LDA   ga6+2
   STA   __zpabi_mix__v_2
   LDA   ga6+3
   JSR   fin0@run1
   LDA   ga6+8
   STA   __zpabi_mix__v_0
   LDA   ga6+9
   STA   __zpabi_mix__v_1
   LDA   ga6+10
   STA   __zpabi_mix__v_2
   LDA   ga6+11
   JSR   fin0@run1
   LDA   ga6+16
   STA   __zpabi_mix__v_0
   LDA   ga6+17
   STA   __zpabi_mix__v_1
   LDA   ga6+18
   STA   __zpabi_mix__v_2
   LDA   ga6+19
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+24
   STA   __zpabi_mix__v_0
   LDA   ga6+25
   STA   __zpabi_mix__v_1
   LDA   ga6+26
   STA   __zpabi_mix__v_2
   LDA   ga6+27
   JSR   fin0@run1
   JSR   fin2@run2
   LDA   gs7+4
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   gs7+4
   STA   __zpabi_mix__v_1
   JMP   mix

fin2@run2:
   SUBROUTINE

   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run1

f10:
   SUBROUTINE

   JSR   fin2@run2
   LDA   #$00
   STA   __zpabi_mix__v_0
   JMP   .loop@0_continue
.f10@rot@0:
   LDA   g4
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_mix__v_0
   LDA   g4
   SEC
   SBC   __local_f10__0
   LDA   __zpabi_mix__v_0
   SBC   #$00
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g1
   AND   #$03
   STA   g1
   LDA   #$FF
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDA   __local_f10__0
   STA   __zpabi_mix__v_0
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f10__0
   LDA   __zpabi_mix__v_0
   CMP   #$02
   BCC   .f10@rot@0
   LDA   __zpabi_f10__p12_0
   STA   __zpabi_mix__v_0
   LDX   #$06
.shift_loop@15:
   LSR   __zpabi_f10__p12_1
   ROR   __zpabi_mix__v_0
   DEX
   BNE   .shift_loop@15
   LDA   __zpabi_mix__v_0
   ORA   __zpabi_f10__p12_1
   BEQ   .if_end@0
   LDA   #$00
   STA   __zpabi_f10__p12_1
   CLC
   JMP   .loop@1_continue
.f10@rot@1:
   LDA   #$00
   STA   g3
   LDA   __zpabi_mix__v_0
   STA   __zpabi_f10__p12_1
.loop@1_continue:
   ADC   #$01
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f10__p12_1
   CMP   #$04
   BCC   .f10@rot@1
.if_end@0:
   LDX   #$00
   LDA   #$01
   RTS

f17:
   SUBROUTINE

   STA   __local_f17__pp18
   STX   __local_f17__4
   LDA   z5
   BNE   .carry_inc@0
   DEC   z5+1
.carry_inc@0:
   DEC   z5
   LDY   #$00
   LDA   (__local_f17__pp18),Y
   STA   __zpabi_mix__v_0
   CLC
   ADC   #$01
   STA   (__local_f17__pp18),Y
   LDA   #$FF
   CLC
   ADC   __zpabi_mix__v_0
   STA   __local_f17__3
   LDA   #$7F
   ADC   #$00
   STA   __local_f17__2
   LDA   #$00
   ADC   #$00
   STA   __zpabi_mix__v_0
   LDA   #$00
   ADC   #$00
   STA   __zpabi_mix__v_1
   LDA   __local_f17__3
   ORA   __local_f17__2
   ORA   __zpabi_mix__v_0
   ORA   __zpabi_mix__v_1
   BEQ   .if_end@1
   LDA   g1
   STA   __zpabi_mix__v_0
   INC   g1
   LDA   (__local_f17__pp18),Y
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   SEC
   SBC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   JSR   fin0@run3
   LDA   z5
   SEC
   SBC   #$FF
   STA   z5
   BCS   .carry_inc@1
   DEC   z5+1
.carry_inc@1:
   EOR   g3
   AND   #$07
   CMP   #$01
   LDA   gs7+1
   SEC
   SBC   #$05
   BCS   .carry_inc@2
   DEC   gs7+2
.carry_inc@2:
.if_end@1:
   LDA   #$00
   STA   gs7+1
   STA   gs7+2
   LDA   g2
   AND   #$05
   STA   g2
   BEQ   .if_end@2
   LDA   g1
   RTS
.if_end@2:
   LDA   gs7+1
   STA   __local_f17__7
   LDA   #$00
   CMP   ga6+23
   BNE   .vthread@0
   CMP   ga6+22
   BNE   .vthread@0
   CMP   ga6+21
   BNE   .vthread@0
   CMP   ga6+20
   BNE   .vthread@0
   CMP   ga6+19
   BNE   .vthread@0
   CMP   ga6+18
   BNE   .vthread@0
   LDA   __local_f17__7
   CMP   ga6+17
   BNE   .thread@0
   CMP   ga6+16
   BEQ   .cmp_true@21
.thread@0:
   LDA   #$00
.vthread@0:
   RTS
.cmp_true@21:
   LDA   #$01
   RTS

f22:
   SUBROUTINE

   STA   __local_f22__p23
   JSR   f22@run0
   CLC
   ADC   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   JSR   fin0@run3
   JSR   f22@run0
   EOR   #$03
   LDY   #$00
   STA   (DPTR),Y
   EOR   __local_f22__p23
   RTS

f22@run0:
   SUBROUTINE

   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l30
   LDA   #$0F
   STA   __local_main__l31
   LDA   #$03
   STA   __zpabi_f22__p24_0
   LDA   #$00
   STA   __zpabi_f22__p24_1
   STA   __zpabi_f22__p24_2
   STA   __zpabi_f22__p24_3
   LDA   #<__local_main__l31
   STA   __zpabi_f22__pp25_0
   LDA   #>__local_main__l31
   STA   __zpabi_f22__pp25_1
   LDA   gs7+1
   JSR   f22
   LDA   __local_main__l30
   BNE   .lb_skip@0
   JMP   .if_end@6
.lb_skip@0:
   LDA   g2
   INC   g2
   ORA   g4
   AND   #$07
   CMP   #$03
   BEQ   .case@8
   CMP   #$04
   BEQ   .case@9
   JMP   .default@10
.case@8:
   JSR   f22@run0
   STA   __local_main__0
   LDA   #$00
   CMP   #$01
   BNE   .cmp_true@28
   CMP   #$00
   BNE   .cmp_true@28
   LDA   __local_main__0
   BNE   .cmp_true@28
   LDA   #$00
   BEQ   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   STA   __local_main__2
   LDA   g4
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_main__1
   STA   g4
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_main__0
   LDA   __local_main__2
   CLC
   ADC   __local_main__1
   STA   __local_main__1
   LDA   #$00
   ADC   __local_main__0
   ORA   __local_main__1
   BEQ   .switch@7_break
   LDX   #>__local_main__l30
   LDA   #<__local_main__l30
   JSR   f17
   JSR   fin0@run4
.case@9:
   LDY   #$00
   LDA   (gp9),Y
   STA   __local_main__1
   CLC
   ADC   #$01
   STA   (gp9),Y
   LDA   __local_main__1
   STA   g4
   JMP   .switch@7_break
.default@10:
   LDA   z5
   EOR   #$03
   STA   z5
.switch@7_break:
.if_end@6:
   LDA   gs7+3
   CLC
   ADC   gs7+3
   LDX   #$FF
   STX   __zpabi_f22__p24_0
   LDX   #$00
   STX   __zpabi_f22__p24_1
   STX   __zpabi_f22__p24_2
   STX   __zpabi_f22__p24_3
   LDX   #<__local_main__l31
   STX   __zpabi_f22__pp25_0
   LDX   #>__local_main__l31
   STX   __zpabi_f22__pp25_1
   JSR   f22
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

ga6:
   DC.L  $00000001
   DC.L  $00000000
   DC.L  $0000FFFF
   DC.L  $00000000
   DC.L  $0000007F
   DC.L  $00000000
   DC.L  $00000100
   DC.L  $00000000

gs7:
   DS.B  3
   DC.W  $000C

gp8:
   DC.W  g1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   LDA   #$FF
   STA   g1
   STA   z5
   STA   z5+1
   LDA   #$80
   STA   g3
   LDA   #$02
   STA   g4
   LDA   #<g2
   STA   gp9
   LDA   #>g2
   STA   gp9+1
   RTS
