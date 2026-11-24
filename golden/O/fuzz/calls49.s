SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$28
__local_f1__0	EQU	$80
__local_main__0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__local_f1__1	EQU	$81
__local_main__1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__local_main__2	EQU	$82
__zpabi_f2__p0_2	EQU	$82
__local_main__3	EQU	$83
__zpabi_f2__p0_3	EQU	$83
__local_f1__4	EQU	$84
__zpabi_f2__p2_0	EQU	$84
__local_f1__5	EQU	$85
__zpabi_f2__p2_1	EQU	$85
__zpabi_f2__p2_2	EQU	$86
__zpabi_f2__p2_3	EQU	$87
__zpabi_f2__p3_0	EQU	$88
__zpabi_f2__p3_1	EQU	$89
__local_f2__0	EQU	$8A
__local_f2__1	EQU	$8B

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0,__local_f0__1 indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7 indirect=false in_cycle=false param_regs=A,X
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3,__zpabi_f2__p3_0,__zpabi_f2__p3_1 locals=__local_f2__0,__local_f2__1 indirect=false in_cycle=false param_regs=,,,,A,,,,,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$C0,$00 weight=9
; zp g0 size=2 at=auto def=true init=$1C,$00 weight=8
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f2:
   SUBROUTINE

   LDA   g0
   ORA   g0+1
   BNE   .or_true@0
   LDA   __zpabi_f2__p0_0
   AND   #$07
   TAX
   LDA   g0
   STA   __local_f2__0
   LDA   g0+1
   STA   __local_f2__1
   CPX   #$00
   BEQ   .shift_done@1
.shift_loop@0:
   ASL   __local_f2__0
   ROL   __local_f2__1
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDA   __local_f2__0
   ORA   __local_f2__1
   BEQ   .or_end@1
.or_true@0:
.or_end@1:
   LDA   g0
   SEC
   SBC   #$02
   LDA   g0+1
   SBC   #$00
   BCS   .and_false@2
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f2__1
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f2__1
.and_end@3:
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f2__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f1:
   SUBROUTINE

   LDA   g1+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f1__5
   STA   __local_f1__4
   LDA   g1+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   g1
   CLC
   SBC   g1
   LDA   g1+1
   SBC   g1+1
   LDA   __local_f1__1
   SBC   __local_f1__5
   LDA   __local_f1__0
   SBC   __local_f1__4
   LDA   #$00
   ROL
   SEC
   SBC   g0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   LDX   #$00
   RTS

f0:
   SUBROUTINE

   LDA   g1+1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   HARGS+10
   STA   HARGS+11
   LDA   g1
   STA   HARGS+8
   LDA   g1+1
   STA   HARGS+9
   RTS

main:
   SUBROUTINE

   JSR   f0
   LDA   g1+1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   __local_main__1
   LDA   #$00
   EOR   __local_main__0
   STA   __local_main__0
   LDA   HARGS+8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   EOR   __local_main__1
   STA   HARGS+2
   LDA   HARGS+11
   EOR   __local_main__0
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS
   LDA   __local_main__2
   STA   HARGS+1
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__3
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__2
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g1+1
   LDA   #$1C
   STA   g0
   LDA   #$C0
   STA   g1
   RTS
