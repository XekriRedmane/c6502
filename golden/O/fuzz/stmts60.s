SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
g2	EQU	$28
g3	EQU	$29
g4	EQU	$2A
__zpabi_f15__p16_0	EQU	$80
__zpabi_f9__pp12_0	EQU	$80
__zpabi_f15__p16_1	EQU	$81
__zpabi_f9__pp12_1	EQU	$81
__zpabi_f15__p16_2	EQU	$82
__zpabi_mix__v_0	EQU	$82
__zpabi_f15__p16_3	EQU	$83
__zpabi_mix__v_1	EQU	$83
__zpabi_f15__p17_0	EQU	$84
__zpabi_mix__v_2	EQU	$84
__zpabi_f15__p17_1	EQU	$85
__zpabi_mix__v_3	EQU	$85
__local_main__0	EQU	$86
__local_main__w25	EQU	$87
__local_main__l24	EQU	$89
__local_f9__l13	EQU	$8B
__local_f9__0	EQU	$8C
__local_f15__2	EQU	$8D
__local_f15__l19	EQU	$8E
__local_f9__2	EQU	$8E
__local_f9__p10	EQU	$8F
__local_mix__0	EQU	$90
__local_mix__1	EQU	$91
__local_mix__2	EQU	$92
__local_mix__3	EQU	$93

; @zp-link-meta-begin
; def f15 params=__zpabi_f15__p16_0,__zpabi_f15__p16_1,__zpabi_f15__p16_2,__zpabi_f15__p16_3,__zpabi_f15__p17_0,__zpabi_f15__p17_1 locals=__local_f15__0,__local_f15__1,__local_f15__2,__local_f15__l19 indirect=false in_cycle=false
; def f9 params=__zpabi_f9__p10,__zpabi_f9__p11,__zpabi_f9__pp12_0,__zpabi_f9__pp12_1 locals=__local_f9__l13,__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__p10 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__w25,__local_main__1,__local_main__l24,__local_main__3 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f15
; call main -> f9
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run1@run3 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run4 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run2 section=.text local=true zero=false
; item f9 section=.text local=false zero=false
; item f15 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=2 at=auto def=true init=$7F,$00
; zp g2 size=1 at=auto def=true init=$00 weight=17
; zp g3 size=1 at=auto def=true init=$FF weight=13
; zp g4 size=1 at=auto def=true init=$45 weight=10
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

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   JSR   fin0@run0
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
   JSR   fin0@run1
   LDA   g4
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   JSR   fin0@run1@run3
   JMP   mix

fin0@run1@run3:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

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

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JSR   fin0@run1@run3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   JSR   fin0@run0
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin0@run0
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   JSR   fin0@run0@run4
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin0@run0
   JSR   fin2@run2
   LDA   gs7+3
   JMP   fin0@tail0

fin2@run2:
   SUBROUTINE

   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   JMP   fin0@run0

f9:
   SUBROUTINE

   STA   __local_f9__p10
   LDA   #$FF
   STA   __local_f9__l13
   STA   __local_f9__0
   LDA   gs7+3
   ORA   #$81
   STA   gs7+3
   STA   __zpabi_mix__v_0
   JSR   fin0@run1
   LDA   gs7+1
   ORA   gs7+2
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f9__2
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f9__2
.or_end@1:
   LDA   __local_f9__p10
   SEC
   SBC   z5
   STA   __local_f9__0
   LDA   #$00
   SBC   z5+1
   STA   __local_f9__l13
   LDA   __local_f9__2
   SEC
   SBC   __local_f9__0
   STA   __local_f9__0
   LDA   #$00
   SBC   __local_f9__l13
   STA   __local_f9__l13
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f9__0
   STA   __zpabi_mix__v_0
   LDA   __local_f9__l13
   STA   __zpabi_mix__v_1
   JSR   mix
   JSR   fin2@run2
   LDA   gs7+1
   RTS

f15:
   SUBROUTINE

   LDA   #$0F
   STA   __local_f15__l19
   LDA   g3
   LSR
   LDA   #$00
   ROR
   STA   g3
   LDA   ga6+4
   LDX   ga6+5
   STX   __local_f15__2
   SEC
   SBC   #$FF
   BCS   .carry_inc@0
   DEC   __local_f15__2
.carry_inc@0:
   STA   ga6+4
   LDX   __local_f15__2
   STX   ga6+5
   ORA   __local_f15__2
   BEQ   .if_end@2
   LDA   __zpabi_f15__p17_0
   EOR   #$FF
   RTS
.if_end@2:
   LDA   ga6
   CLC
   ADC   __local_f15__l19
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l24
   LDX   #$06
.shift_loop@6:
   LSR   g1+1
   ROR   g1
   DEX
   BNE   .shift_loop@6
   LDA   g1
   ORA   g1+1
   BEQ   .if_end@6
   LDA   #$1F
   STA   __zpabi_mix__v_0
   JSR   fin0@run1
   LDA   #$00
   STA   __local_main__w25
   JMP   .loop@5_continue
.main@rot@0:
   LDA   g2
   ORA   #$5A
   STA   g2
   AND   g4
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   __local_main__0
   STA   __local_main__w25
.loop@5_continue:
   CLC
   ADC   #$01
   STA   __local_main__0
   LDA   __local_main__w25
   BEQ   .main@rot@0
   LDA   ga6+4
   STA   ga6+2
   LDA   ga6+5
   STA   ga6+3
.if_end@6:
   LDA   g1
   SEC
   SBC   #$03
   STA   g1
   BCS   .carry_inc@1
   DEC   g1+1
.carry_inc@1:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_main__0
   LDA   #$80
   SEC
   SBC   __local_main__0
   STA   __local_main__w25
   LDA   #$00
   SBC   #$00
   STA   __local_main__0
   LDA   g1
   SEC
   SBC   __local_main__w25
   STA   __zpabi_mix__v_0
   LDA   g1+1
   SBC   __local_main__0
   JSR   fin0@run0
   LDA   gs7+3
   STA   HARGS
   LDA   z5
   STA   HARGS+1
   JSR   mul8
   LDA   #<__local_main__l24
   STA   __zpabi_f9__pp12_0
   LDA   #>__local_main__l24
   STA   __zpabi_f9__pp12_1
   LDX   #$FF
   LDA   HARGS+2
   JSR   f9
   LDA   ga6+6
   STA   __local_main__w25
   LDA   ga6+7
   STA   __local_main__0
   SEC
   LDA   #$00
   SBC   __local_main__w25
   STA   __zpabi_f15__p16_0
   LDA   #$00
   SBC   __local_main__0
   STA   __zpabi_f15__p16_1
   LDA   gs7+1
   AND   g4
   STA   __zpabi_f15__p17_0
   LDA   #$00
   STA   __zpabi_f15__p16_2
   STA   __zpabi_f15__p16_3
   STA   __zpabi_f15__p17_1
   JSR   f15
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
   DC.W  $00FF

ga6:
   DC.W  $0080
   DC.W  $0002
   DC.W  $0003
   DC.W  $FFFF

gs7:
   DS.B  3
   DC.B  $7F

gp8:
   DC.W  g4

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   z5+1
   LDA   #$FF
   STA   g3
   LDA   #$45
   STA   g4
   LDA   #$7F
   STA   z5
   RTS
