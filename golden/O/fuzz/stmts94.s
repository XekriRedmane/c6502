SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
g4	EQU	$27
__zpabi_f10__pp12_0	EQU	$80
__zpabi_f10__pp12_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__0	EQU	$86
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__l24	EQU	$8E
__local_f10__w16	EQU	$92
__local_f17__0	EQU	$92
__local_f10__0	EQU	$93
__local_f17__1	EQU	$93
__local_f10__1	EQU	$94
__local_f17__l18	EQU	$94
__local_f10__2	EQU	$95
__local_f17__2	EQU	$95
__local_f10__3	EQU	$96
__local_f10__p11_1	EQU	$99
__local_f10__p11_0	EQU	$9A
__local_mix__0	EQU	$9C
__local_mix__1	EQU	$9D
__local_mix__2	EQU	$9E
__local_mix__3	EQU	$9F

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1,__zpabi_f10__pp12_0,__zpabi_f10__pp12_1 locals=__local_f10__w16,__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3,__local_f10__4,__local_f10__5,__local_f10__p11_1,__local_f10__p11_0,__local_f10__6 indirect=false in_cycle=false param_regs=A,X,,
; def f17 params= locals=__local_f17__0,__local_f17__1,__local_f17__l18,__local_f17__2,__local_f17__3 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__l24,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f17 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f10
; call main -> f17
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f17 section=.text local=false zero=false
; item f17@run4 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item @7.s21 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=1 at=auto def=true init=$00
; zp g4 size=1 at=auto def=true init=$03 weight=6
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
   STA   __zpabi_mix__v_0
   JSR   fin0@run3
   LDA   g3+5
   LDA   g3+6
   LDA   g3+7
   JSR   mix
   LDA   g4
   JSR   fin0@run5
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin0@run3:
   SUBROUTINE

   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   STA   __zpabi_mix__v_3
   LDA   g3+4
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   JSR   fin1@run2
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   STA   __zpabi_mix__v_1
   LDA   ga6+2
   STA   __zpabi_mix__v_2
   LDA   ga6+3
   JSR   fin0@run0
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   LDA   ga6+6
   STA   __zpabi_mix__v_2
   LDA   ga6+7
   JSR   fin0@run0
   LDA   ga6+8
   STA   __zpabi_mix__v_0
   LDA   ga6+9
   STA   __zpabi_mix__v_1
   LDA   ga6+10
   STA   __zpabi_mix__v_2
   LDA   ga6+11
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run2:
   SUBROUTINE

   JSR   fin0@run5
   JMP   fin0@run0

fin2:
   SUBROUTINE

   LDA   ga6+12
   STA   __zpabi_mix__v_0
   LDA   ga6+13
   STA   __zpabi_mix__v_1
   LDA   ga6+14
   STA   __zpabi_mix__v_2
   LDA   ga6+15
   JSR   fin0@run0
   LDA   gs7+1
   JSR   fin1@run2
   LDA   gs7+2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+2
   STA   __zpabi_mix__v_0
   JMP   mix

f10:
   SUBROUTINE

   STA   __local_f10__p11_0
   STX   __local_f10__p11_1
   LDA   #$00
   STA   __local_f10__w16
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f10__3
   LDA   __local_f10__w16
   BEQ   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   gs7+1
   AND   #$01
   STA   gs7+1
   LDA   __local_f10__3
   ASL
   LDA   #$00
   ROL
   JSR   fin1@run2
   LDY   #$03
   LDA   (__zpabi_f10__pp12_0),Y
   STA   __local_f10__0
   LDY   #$00
   LDA   (__zpabi_f10__pp12_0),Y
   SEC
   SBC   #$5A
   STA   __local_f10__w16
   INY
   LDA   (__zpabi_f10__pp12_0),Y
   SBC   #$00
   STA   __local_f10__2
   INY
   LDA   (__zpabi_f10__pp12_0),Y
   SBC   #$00
   STA   __local_f10__1
   BCS   .carry_inc@0
   DEC   __local_f10__0
.carry_inc@0:
   LDA   __local_f10__w16
   LDY   #$00
   STA   (__zpabi_f10__pp12_0),Y
   LDA   __local_f10__2
   INY
   STA   (__zpabi_f10__pp12_0),Y
   LDA   __local_f10__1
   INY
   STA   (__zpabi_f10__pp12_0),Y
   LDA   __local_f10__0
   INY
   STA   (__zpabi_f10__pp12_0),Y
   LDA   #$00
   SEC
   SBC   __local_f10__w16
   STA   __zpabi_mix__v_0
   LDA   #$80
   SBC   __local_f10__2
   STA   __zpabi_mix__v_1
   LDA   #$00
   SBC   __local_f10__1
   STA   __zpabi_mix__v_2
   LDA   #$00
   SBC   __local_f10__0
   JSR   fin0@run0
   LDA   __local_f10__3
   STA   __local_f10__w16
   JMP   .loop@0_continue
.loop@0_break:
   LDA   z5
   DEC   z5
   STA   gs7+2
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_f10__0
   STA   __local_f10__w16
   LDA   ga6+4
   SEC
   SBC   g4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   ga6+6
   SBC   __local_f10__0
   STA   __zpabi_mix__v_2
   LDA   ga6+7
   SBC   __local_f10__w16
   JSR   fin0@run0
   LDA   #$09
   EOR   __local_f10__p11_0
   STA   __local_f10__w16
   LDA   #$00
   EOR   __local_f10__p11_1
   TAX
   LDA   __local_f10__w16
   RTS

f17:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f17__l18
   STA   __local_f17__2
   LDA   #$03
   CLC
   ADC   @7.s21
   STA   __zpabi_mix__v_0
   LDA   #$00
   ADC   @7.s21+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   ADC   @7.s21+2
   STA   __zpabi_mix__v_2
   LDA   #$00
   ADC   @7.s21+3
   JSR   fin0@run0
   INC   gs7+1
   LDA   __local_f17__l18
   STA   __local_f17__0
   LDA   __local_f17__2
   STA   __local_f17__1
   LDA   __local_f17__0
   BNE   .carry_inc@1
   DEC   __local_f17__1
.carry_inc@1:
   DEC   __local_f17__0
   LDA   __local_f17__0
   STA   __local_f17__l18
   LDA   __local_f17__1
   STA   __local_f17__2
   LDA   #$FF
   STA   gs7+1
   JSR   f17@run4
   SEC
   SBC   z5
   STA   __local_f17__0
   LDA   #$00
   SBC   #$00
   STA   __local_f17__1
   LDA   #$FF
   EOR   __local_f17__0
   STA   __local_f17__0
   LDA   #$00
   EOR   __local_f17__1
   TAX
   LDA   __local_f17__0
   RTS

f17@run4:
   SUBROUTINE

   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__l24
   LDA   #$00
   STA   __local_main__l24+1
   STA   __local_main__l24+2
   STA   __local_main__l24+3
   LDA   g1
   ORA   #$03
   STA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BEQ   .if_else@2
   JSR   f17@run4
   INC   g2
   BNE   .inc_done@0
   INC   g2+1
   BNE   .inc_done@0
   INC   g2+2
   BNE   .inc_done@0
   INC   g2+3
   BNE   .inc_done@0
   INC   g2+4
   BNE   .inc_done@0
   INC   g2+5
   BNE   .inc_done@0
   INC   g2+6
   BNE   .inc_done@0
   INC   g2+7
.inc_done@0:
   SEC
   SBC   ga6
   ORA   g2
   STA   g4
   JMP   .if_end@1
.if_else@2:
   LDA   g4
   STA   __local_main__3
   LDA   #$00
   SEC
   SBC   __local_main__3
   STA   __local_main__2
   BCS   .carry_inc@2
   DEC   __local_main__3
.carry_inc@2:
   LDA   __local_main__3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_0
   STA   __local_main__0
   JSR   main@run1
   LDA   __local_main__2
   LDY   #$00
   STA   (DPTR),Y
   JSR   main@run1
   LDA   __local_main__3
   LDY   #$01
   STA   (DPTR),Y
   JSR   main@run1
   LDA   __zpabi_mix__v_0
   LDY   #$02
   STA   (DPTR),Y
   JSR   main@run1
   LDA   __local_main__0
   LDY   #$03
   STA   (DPTR),Y
   LDA   #$00
   STA   z5
.if_end@1:
   LDA   g4
   BEQ   .if_end@3
   LDA   g3
   JSR   fin0@run3
   LDX   g3+5
   STX   __local_main__2
   LDX   g3+6
   STX   __zpabi_mix__v_0
   LDX   g3+7
   STX   __local_main__0
   LDX   #$FF
   STX   g3
   LDX   __zpabi_mix__v_1
   STX   g3+1
   LDX   __zpabi_mix__v_2
   STX   g3+2
   LDX   __zpabi_mix__v_3
   STX   g3+3
   STA   g3+4
   LDA   __local_main__2
   STA   g3+5
   LDA   __zpabi_mix__v_0
   STA   g3+6
   LDA   __local_main__0
   STA   g3+7
   LDA   #$FF
   STA   __zpabi_mix__v_0
   JSR   mix
   JSR   f17
   STA   __zpabi_mix__v_0
   TXA
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   STX   __zpabi_mix__v_1
   JSR   mix
.if_end@3:
   LDA   g2
   STA   __local_main__2
   LDX   g2+1
   LDA   #<__local_main__l24
   STA   __zpabi_f10__pp12_0
   LDA   #>__local_main__l24
   STA   __zpabi_f10__pp12_1
   LDA   __local_main__2
   JSR   f10
   LDA   __local_main__l24
   SEC
   SBC   #$01
   STA   __local_main__3
   LDA   __local_main__l24+1
   SBC   #$00
   STA   __local_main__2
   LDA   __local_main__l24+2
   SBC   #$00
   STA   __zpabi_mix__v_0
   LDA   __local_main__l24+3
   SBC   #$00
   LDX   __local_main__3
   STX   __local_main__l24
   LDX   __local_main__2
   STX   __local_main__l24+1
   LDX   __zpabi_mix__v_0
   STX   __local_main__l24+2
   STA   __local_main__l24+3
   LDA   __local_main__3
   AND   #$07
   CMP   #$01
   BEQ   .switch@1_break
   CMP   #$04
   BEQ   .case@3
   CMP   #$05
   BEQ   .case@4
   CMP   #$07
   BEQ   .case@5
   BNE   .switch@1_break
.case@3:
   LDA   g2
   ORA   g2+1
   ORA   g2+2
   ORA   g2+3
   ORA   g2+4
   ORA   g2+5
   ORA   g2+6
   ORA   g2+7
   BNE   .switch@1_break
   LDA   ga6+8
   ORA   ga6+9
   ORA   ga6+10
   ORA   ga6+11
   BEQ   .switch@1_break
.case@4:
.case@5:
.switch@1_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run1:
   SUBROUTINE

   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   RTS

acc:
   DS.B  2

g1:
   DC.L  $00008000

g2:
   DC.L  $00007FFF
   DC.L  $00000000

g3:
   DC.L  $7FFFFFFF
   DC.L  $00000000

ga6:
   DC.L  $00010000
   DC.L  $00010000
   DC.L  $7FFFFFFF
   DC.L  $18AC3833

gs7:
   DS.B  1
   DC.B  $FF
   DC.B  $FF

gp8:
   DC.W  g1

gp9:
   DC.W  z5

@7.s21:
   DC.L  $00000001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z5
   LDA   #$03
   STA   g4
   RTS
