SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$2E
z7	EQU	$35
g4	EQU	$3D
gp10	EQU	$3E
g1	EQU	$40
gp11	EQU	$47
__zpabi_f12__p14_0	EQU	$80
__zpabi_f12__p14_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__0	EQU	$86
__local_main__w24	EQU	$87
__local_main__1	EQU	$88
__local_main__3	EQU	$8A
__local_main__4	EQU	$8B
__local_main__5	EQU	$8C
__local_main__6	EQU	$8D
__local_f12__l17	EQU	$8E
__local_f12__0	EQU	$8F
__local_f12__1	EQU	$90
__local_f12__2	EQU	$91
__local_f12__3	EQU	$92
__local_f12__4	EQU	$93
__local_f12__5	EQU	$94
__local_f12__6	EQU	$95
__local_f12__l16	EQU	$96
__local_f12__7	EQU	$97
__local_mix__0	EQU	$98
__local_mix__1	EQU	$99
__local_mix__2	EQU	$9A
__local_mix__3	EQU	$9B

; @zp-link-meta-begin
; def f12 params=__zpabi_f12__p13_0,__zpabi_f12__p13_1,__zpabi_f12__p14_0,__zpabi_f12__p14_1 locals=__local_f12__l17,__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__3,__local_f12__4,__local_f12__5,__local_f12__6,__local_f12__l16,__local_f12__7 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def fin3 params= locals=__local_fin3__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__w24,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f12 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call fin3 -> mix
; call main -> f12
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> fin3
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin0@tail0@tail1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin3 section=.text local=false zero=false
; item f12 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item ga8 section=.data local=false zero=false
; item gs9 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=8 at=auto def=true init=$00,$01,$00,$00,$00,$00,$00,$00
; zp z6 size=7 at=auto def=true init=$FF,$7F,$00,$00,$00,$00,$00
; zp z7 size=8 at=auto def=true init=$03,$00,$00,$00,$00,$00,$00,$00
; zp g4 size=1 at=auto def=true init=$03 weight=386
; zp gp10 size=2 at=auto def=true init=<z5,>z5 weight=129
; zp g1 size=7 at=auto def=true init=$03,$00,$00,$00,$00,$00,$00 weight=73
; zp gp11 size=2 at=auto def=true init=<g2,>g2 weight=18
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
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run0
   LDA   g3
   JSR   fin0@run1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   g4
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run1
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0@tail1

fin0@tail0@tail1:
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
   LDA   z7
   STA   __zpabi_mix__v_0
   LDA   z7+1
   STA   __zpabi_mix__v_1
   LDA   z7+2
   STA   __zpabi_mix__v_2
   LDA   z7+3
   STA   __zpabi_mix__v_3
   LDA   z7+4
   LDA   z7+5
   LDA   z7+6
   LDA   z7+7
   JSR   mix
   LDA   ga8
   STA   __zpabi_mix__v_0
   LDA   ga8+1
   STA   __zpabi_mix__v_1
   LDA   ga8+2
   STA   __zpabi_mix__v_2
   LDA   ga8+3
   JMP   fin0@tail0@tail1

fin2:
   SUBROUTINE

   LDA   ga8+4
   STA   __zpabi_mix__v_0
   LDA   ga8+5
   STA   __zpabi_mix__v_1
   LDA   ga8+6
   STA   __zpabi_mix__v_2
   LDA   ga8+7
   JSR   fin0@run0
   LDA   ga8+8
   STA   __zpabi_mix__v_0
   LDA   ga8+9
   STA   __zpabi_mix__v_1
   LDA   ga8+10
   STA   __zpabi_mix__v_2
   LDA   ga8+11
   JSR   fin0@run0
   LDA   ga8+12
   STA   __zpabi_mix__v_0
   LDA   ga8+13
   STA   __zpabi_mix__v_1
   LDA   ga8+14
   STA   __zpabi_mix__v_2
   LDA   ga8+15
   JSR   fin0@run0
   LDA   gs9+1
   STA   __zpabi_mix__v_0
   LDA   gs9+2
   STA   __zpabi_mix__v_1
   LDA   gs9+3
   STA   __zpabi_mix__v_2
   LDA   gs9+4
   JMP   fin0@tail0@tail1

fin3:
   SUBROUTINE

   LDA   gs9+8
   JMP   fin0@tail0

f12:
   SUBROUTINE

   LDA   #$7F
   STA   __local_f12__l16
   LDA   #$00
   STA   __local_f12__7
   LDA   #$01
   STA   __local_f12__l17
   LDA   #$00
   STA   __local_f12__0
   LDY   #$00
   LDA   (gp10),Y
   STA   __local_f12__6
   INY
   LDA   (gp10),Y
   STA   __local_f12__5
   INY
   LDA   (gp10),Y
   STA   __local_f12__4
   INY
   LDA   (gp10),Y
   STA   __local_f12__3
   LDA   ga8+4
   STA   HARGS
   LDA   ga8+5
   STA   HARGS+1
   LDA   ga8+6
   STA   HARGS+2
   LDA   ga8+7
   STA   HARGS+3
   LDA   #$5A
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   ga8+4
   LDA   HARGS+9
   STA   ga8+5
   LDA   HARGS+10
   STA   ga8+6
   LDA   HARGS+11
   STA   ga8+7
   LDA   __local_f12__6
   ORA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   __local_f12__5
   ORA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   __local_f12__4
   ORA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   __local_f12__3
   ORA   HARGS+11
   JSR   fin0@run0
   LDA   #$FF
   STA   g3
   LDA   __local_f12__l16
   STA   __local_f12__2
   LDA   __local_f12__7
   STA   __local_f12__1
   LDY   #$00
   LDA   (gp11),Y
   STA   __local_f12__l17
   INY
   LDA   (gp11),Y
   STA   __local_f12__0
   LDA   __local_f12__2
   SEC
   SBC   __local_f12__l17
   STA   __local_f12__l17
   LDA   __local_f12__1
   SBC   __local_f12__0
   TAX
   LDA   __local_f12__l17
   RTS

main:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDX   ga8
   STX   __zpabi_mix__v_0
   LDX   ga8+1
   STX   __local_main__1
   LDX   ga8+2
   STX   __local_main__w24
   CMP   ga8+3
   BNE   .cmp_true@2
   CMP   __local_main__w24
   BNE   .cmp_true@2
   LDA   #$00
   CMP   __local_main__1
   BNE   .cmp_true@2
   LDA   g4
   CMP   __zpabi_mix__v_0
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   STA   __local_main__0
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__0
   JSR   fin0@run1
   JSR   mix
   LDA   ga8
   STA   __local_main__5
   LDA   ga8+1
   STA   __local_main__4
   LDA   ga8+2
   STA   __local_main__3
   LDA   ga8+3
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_main__1
   STA   __local_main__w24
   AND   #$1F
   STA   __local_main__0
   LDA   __local_main__5
   SEC
   SBC   #$01
   LDA   __local_main__4
   SBC   #$00
   LDA   __local_main__3
   SBC   #$01
   LDA   __zpabi_mix__v_0
   SBC   #$00
   LDA   __local_main__1
   SBC   #$00
   LDA   __local_main__w24
   SBC   #$00
   LDA   __local_main__0
   SBC   #$00
   BCC   .if_else@2
   LDA   gs9+1
   ORA   gs9+2
   ORA   gs9+3
   ORA   gs9+4
   ORA   gs9+5
   ORA   gs9+6
   ORA   gs9+7
   BEQ   .lnot_true@9
   LDA   #$00
   BEQ   .lnot_end@10
.lnot_true@9:
   LDA   #$01
.lnot_end@10:
   STA   __local_main__0
   LDA   #$00
   STA   __zpabi_f12__p14_0
   STA   __zpabi_f12__p14_1
   LDX   #$00
   LDA   __local_main__0
   JSR   f12
   JMP   .if_end@1
.if_else@2:
   LDA   ga8+8
   AND   #$03
   STA   __zpabi_mix__v_0
   LDA   #$00
   LDY   #$00
   STY   ga8+9
   STY   ga8+10
   STY   ga8+11
   STY   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_0
   STA   ga8+8
   STY   __zpabi_mix__v_3
   JSR   mix
.if_end@1:
   LDY   #$00
   LDA   (gp11),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp11),Y
   STA   __local_main__1
   INY
   LDA   (gp11),Y
   STA   __local_main__w24
   INY
   LDA   (gp11),Y
   STA   __local_main__0
   LDA   #$81
   SEC
   SBC   __zpabi_mix__v_0
   STA   __local_main__6
   LDA   #$FF
   SBC   __local_main__1
   STA   __local_main__5
   LDA   #$FF
   SBC   __local_main__w24
   STA   __local_main__4
   LDA   #$FF
   SBC   __local_main__0
   STA   __local_main__3
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __zpabi_mix__v_0
   STA   __local_main__1
   STA   __local_main__w24
   STA   __local_main__0
   LDA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __zpabi_mix__v_0
   ORA   __local_main__1
   ORA   __local_main__w24
   ORA   __local_main__0
   BEQ   .if_end@3
   LDA   #$00
   STA   __local_main__w24
   JMP   .loop@8_continue
.main@rot@0:
   LDA   __local_main__0
   BNE   .loop@8_break
   STA   __local_main__w24
.loop@8_continue:
   CLC
   ADC   #$01
   STA   __local_main__0
   LDA   __local_main__w24
   BEQ   .main@rot@0
.loop@8_break:
.if_end@3:
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

g2:
   DS.B  4

g3:
   DC.B  $FF

ga8:
   DC.L  $80000000
   DC.L  $0000007F
   DC.L  $00010000
   DC.L  $F06A2798

gs9:
   DS.B  1
   DC.B  $0C
   DS.B  6
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g1+4
   STA   g1+5
   STA   g1+6
   STA   z5
   STA   z5+2
   STA   z5+3
   STA   z5+4
   STA   z5+5
   STA   z5+6
   STA   z5+7
   STA   z6+2
   STA   z6+3
   STA   z6+4
   STA   z6+5
   STA   z6+6
   STA   z7+1
   STA   z7+2
   STA   z7+3
   STA   z7+4
   STA   z7+5
   STA   z7+6
   STA   z7+7
   LDA   #$03
   STA   g1
   STA   g4
   STA   z7
   LDA   #$01
   STA   z5+1
   LDA   #$FF
   STA   z6
   LDA   #$7F
   STA   z6+1
   LDA   #<z5
   STA   gp10
   LDA   #>z5
   STA   gp10+1
   LDA   #<g2
   STA   gp11
   LDA   #>g2
   STA   gp11+1
   RTS
