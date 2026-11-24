SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g3	EQU	$26
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__1	EQU	$85
__local_main__2	EQU	$86
__local_main__3	EQU	$87
__local_main__4	EQU	$88
__local_main__5	EQU	$89
__local_main__6	EQU	$8A
__local_main__7	EQU	$8B
__local_main__l12	EQU	$8C
__local_f7__1	EQU	$8E
__local_f7__2	EQU	$8F
__local_f7__3	EQU	$90
__local_f7__4	EQU	$91
__local_f7__5	EQU	$92
__local_f7__6	EQU	$93
__local_f7__pp8	EQU	$94
__local_f7__7	EQU	$95
__local_mix__0	EQU	$96
__local_mix__1	EQU	$97
__local_mix__2	EQU	$98
__local_mix__3	EQU	$99

; @zp-link-meta-begin
; def f7 params=__zpabi_f7__pp8_0,__zpabi_f7__pp8_1 locals=__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__3,__local_f7__4,__local_f7__5,__local_f7__6,__local_f7__pp8,__local_f7__7 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__l12 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin0@tail0@run4 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item f7@run2 section=.text local=true zero=false
; item f7@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item gp6 section=.data local=false zero=false
; item @2.s10 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp g3 size=1 at=auto def=true init=$FF weight=7
; runtime hargs_size=32 hargs_need=5
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

   JSR   fin0@run3
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   JMP   fin0@tail0

fin0@run3:
   SUBROUTINE

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@tail0@run4
   JMP   mix

fin0@tail0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga4+2
   STA   __zpabi_mix__v_0
   LDA   ga4+3
   JSR   fin1@run0
   LDA   ga4+4
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   JSR   fin1@run0
   LDA   ga4+6
   STA   __zpabi_mix__v_0
   LDA   ga4+7
   JSR   fin1@run0
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   JMP   fin0@tail0

fin1@run0:
   SUBROUTINE

   JSR   fin0@tail0@run4
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs5+3
   STA   __zpabi_mix__v_0
   LDA   gs5+4
   JMP   fin0@tail0

f7:
   SUBROUTINE

   STA   __local_f7__pp8
   STX   __local_f7__7
   LDA   g2+6
   STA   __local_f7__6
   LDA   g2+5
   STA   __local_f7__5
   LDA   g2+4
   STA   __local_f7__4
   LDA   g2+3
   STA   __local_f7__3
   LDA   g2+2
   STA   __local_f7__2
   LDA   g2+1
   STA   __local_f7__1
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   #$00
   SEC
   SBC   g2
   STA   g2
   LDA   __zpabi_mix__v_0
   SBC   g2+1
   STA   g2+1
   LDA   __local_f7__1
   SBC   g2+2
   STA   g2+2
   LDA   __local_f7__2
   SBC   g2+3
   STA   g2+3
   LDA   __local_f7__3
   SBC   g2+4
   STA   g2+4
   LDA   __local_f7__4
   SBC   g2+5
   STA   g2+5
   LDA   __local_f7__5
   SBC   g2+6
   STA   g2+6
   LDA   __local_f7__6
   SBC   g2+7
   STA   g2+7
   LDA   g3
   AND   #$03
   STA   __zpabi_mix__v_0
   STA   g3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   f7@run1
   LDX   #$06
.shift_loop@4:
   LDA   g3
   ASL
   ROR   g3
   DEX
   BNE   .shift_loop@4
   LDY   #$00
   LDA   (__local_f7__pp8),Y
   ASL
   ASL
   ASL
   STA   (__local_f7__pp8),Y
   JSR   f7@run2
   LDA   #$FF
   STA   @2.s10
   STA   @2.s10+1
   STA   @2.s10+2
   LDA   #$7F
   STA   @2.s10+3
   LDY   #$00
   LDA   (__local_f7__pp8),Y
   LDX   #$FF
   SEC
.div_loop@6:
   INX
   SBC   #$05
   BCS   .div_loop@6
   ADC   #$05
   LDY   #$00
   STA   (DPTR),Y
   LDA   g3
   STA   HARGS
   LDA   @2.s10
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   RTS

f7@run2:
   SUBROUTINE

   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

f7@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

main:
   SUBROUTINE

   LDA   #$07
   STA   __local_main__l12
   LDX   #>__local_main__l12
   LDA   #<__local_main__l12
   JSR   f7
   STA   __zpabi_mix__v_0
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   JSR   f7@run1
   LDA   g2+7
   ORA   g2+6
   ORA   g2+5
   ORA   g2+4
   ORA   g2+3
   ORA   g2+2
   ORA   g2+1
   ORA   g2
   BEQ   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@12:
   LDA   #$01
.cmp_end@11:
   AND   gs5+1
   BNE   .lb_skip@1
   JMP   .if_end@2
.lb_skip@1:
   LDX   #>__local_main__l12
   LDA   #<__local_main__l12
   JSR   f7
   LDA   ga4+2
   STA   __local_main__1
   LDA   ga4+3
   LSR
   LDA   __local_main__1
   ROR
   STA   __local_main__2
   LDA   #$00
   ROR
   STA   __zpabi_mix__v_0
   STA   ga4+2
   LDA   __local_main__2
   STA   ga4+3
   LDA   __zpabi_mix__v_0
   ORA   #$88
   STA   __local_main__1
   LDA   __local_main__2
   ORA   #$B8
   ORA   __local_main__1
   BNE   .lb_skip@0
   JMP   .if_else@4
.lb_skip@0:
   LDA   g2+7
   LSR
   LDA   g2+6
   ROR
   STA   __local_main__7
   LDA   g2+5
   ROR
   STA   __local_main__6
   LDA   g2+4
   ROR
   STA   __local_main__5
   LDA   g2+3
   ROR
   STA   __local_main__4
   LDA   g2+2
   ROR
   STA   __local_main__3
   LDA   g2+1
   ROR
   STA   __local_main__2
   LDA   g2
   ROR
   STA   __local_main__1
   LDA   #$00
   ROR
   CLC
   ADC   g2
   STA   g2
   LDA   __local_main__1
   ADC   g2+1
   STA   g2+1
   LDA   __local_main__2
   ADC   g2+2
   STA   g2+2
   LDA   __local_main__3
   ADC   g2+3
   STA   g2+3
   LDA   __local_main__4
   ADC   g2+4
   STA   g2+4
   LDA   __local_main__5
   ADC   g2+5
   STA   g2+5
   LDA   __local_main__6
   ADC   g2+6
   STA   g2+6
   LDA   __local_main__7
   ADC   g2+7
   STA   g2+7
   LDX   #>__local_main__l12
   LDA   #<__local_main__l12
   JSR   f7
   LDA   g2
   CLC
   ADC   #$01
   STA   __local_main__7
   LDA   g2+1
   ADC   #$00
   STA   __local_main__6
   LDA   g2+2
   ADC   #$00
   STA   __local_main__5
   LDA   g2+3
   ADC   #$00
   STA   __local_main__4
   LDA   g2+4
   ADC   #$00
   STA   __local_main__3
   LDA   g2+5
   ADC   #$00
   STA   __local_main__2
   LDA   g2+6
   ADC   #$00
   STA   __local_main__1
   LDA   g2+7
   ADC   #$00
   STA   __zpabi_mix__v_0
   LDA   __local_main__7
   STA   g2
   LDA   __local_main__6
   STA   g2+1
   LDA   __local_main__5
   STA   g2+2
   LDA   __local_main__4
   STA   g2+3
   LDA   __local_main__3
   STA   g2+4
   LDA   __local_main__2
   STA   g2+5
   LDA   __local_main__1
   STA   g2+6
   LDA   __zpabi_mix__v_0
   STA   g2+7
   LDA   __local_main__7
   STA   g1
   LDA   __local_main__6
   STA   g1+1
   LDA   __local_main__5
   STA   g1+2
   LDA   __local_main__4
   STA   g1+3
   JMP   .if_end@3
.if_else@4:
   JSR   f7@run2
   SEC
   SBC   #$05
   STA   __zpabi_mix__v_0
   LDY   #$00
   STA   (DPTR),Y
   LDA   gs5+3
   ASL
   ASL
   ASL
   ORA   __zpabi_mix__v_0
   AND   #$07
   CMP   #$01
   BEQ   .case@4
   CMP   #$07
   BNE   .switch@3_break
   JSR   fin0@run3
   LDA   g1+2
   LDA   g1+3
   LDA   __zpabi_mix__v_0
   STA   g3
.case@4:
.switch@3_break:
.if_end@3:
   LDX   #>__local_main__l12
   LDA   #<__local_main__l12
   JSR   f7
   STA   __zpabi_mix__v_0
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   JSR   f7@run1
.if_end@2:
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
   DC.L  $0000007F

g2:
   DC.L  $0000FFFF
   DC.L  $00000000

ga4:
   DS.B  4
   DC.W  $FFFF
   DC.W  $FFFF

gs5:
   DS.B  1
   DC.W  $0100
   DS.B  2

gp6:
   DC.W  g3

@2.s10:
   DC.L  $80000000

__zp_init:
   SUBROUTINE

   LDA   #$FF
   STA   g3
   RTS
