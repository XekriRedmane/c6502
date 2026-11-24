SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
__zpabi_f0__p0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f0__p1_0	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f0__p1_1	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f0__p1_2	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f0__p1_3	EQU	$84
__zpabi_f1__p1_0	EQU	$84
__local_main__0	EQU	$85
__zpabi_f1__p1_1	EQU	$85
__local_main__1	EQU	$86
__zpabi_f1__p1_2	EQU	$86
__local_main__2	EQU	$87
__zpabi_f1__p1_3	EQU	$87
__local_main__3	EQU	$88
__zpabi_f2__p0_0	EQU	$88
__local_main__4	EQU	$89
__zpabi_f2__p0_1	EQU	$89
__local_main__5	EQU	$8A
__zpabi_f2__p0_2	EQU	$8A
__local_main__6	EQU	$8B
__zpabi_f2__p0_3	EQU	$8B
__local_main__7	EQU	$8C
__zpabi_f2__pp_0	EQU	$8C
__local_main__8	EQU	$8D
__zpabi_f2__pp_1	EQU	$8D
__local_f1__0	EQU	$8E
__local_f1__l1	EQU	$8F
__local_f1__2	EQU	$91
__local_f1__box2	EQU	$94
__local_f2__0	EQU	$97
__local_f2__1	EQU	$98

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2_0,__zpabi_f0__p2_1 locals= indirect=false in_cycle=false param_regs=,,,,,A,X
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3 locals=__local_f1__0,__local_f1__l1,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__box2,__local_f1__6,__local_f1__7 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f1 -> f2
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=1 at=auto def=true init=$A8 weight=3
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f2:
   SUBROUTINE

   LDA   __zpabi_f2__p0_0
   AND   g1
   STA   __local_f2__1
   LDA   __zpabi_f2__p0_1
   AND   g1+1
   STA   __local_f2__0
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   CLC
   ADC   __local_f2__1
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   ADC   __local_f2__0
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   DEY
   STA   (__zpabi_f2__pp_0),Y
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f2__0
   LDA   #$00
   STA   __local_f2__1
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f2__0
   STA   __local_f2__1
.and_end@1:
   LDA   __zpabi_f2__p0_0
   AND   #$07
   TAX
   BEQ   .shift_done@1
.shift_loop@0:
   ASL   __local_f2__0
   ROL   __local_f2__1
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDA   __local_f2__1
   ORA   __local_f2__0
   BEQ   .cmp_end@2
   SEC
   RTS
.cmp_end@2:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p0_0
   EOR   g1
   STA   __local_f1__2
   LDA   __zpabi_f1__p0_1
   EOR   g1+1
   STA   __zpabi_f2__p0_0
   LDA   __zpabi_f1__p0_2
   EOR   g1+2
   STA   __local_f1__l1
   LDA   __zpabi_f1__p0_3
   EOR   g1+3
   STA   __local_f1__0
   LDA   #$00
   CMP   __local_f1__0
   BNE   .cmp_end@4
   CMP   __local_f1__l1
   BNE   .cmp_end@4
   LDA   g0+1
   CMP   __zpabi_f2__p0_0
   BNE   .thread@0
   LDA   g0
   CMP   __local_f1__2
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_f1__2
   LDA   __zpabi_f1__p0_0
   STA   __local_f1__box2
   LDA   #$00
   STA   __local_f1__box2+1
   LDA   __local_f1__2
   CMP   __zpabi_f1__p0_0
   BNE   .cmp_true@6
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __zpabi_f2__p0_0
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   LDA   #$00
   STA   __zpabi_f2__p0_1
   LDA   #<__local_f1__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f1__box2
   STA   __zpabi_f2__pp_1
   JSR   f2
   BCC   .if_else@3
   LDA   __local_f1__2
   CLC
   ADC   #$03
   JMP   .if_end@2
.if_else@3:
   LDA   __local_f1__2
   EOR   #$05
.if_end@2:
   CLC
   ADC   __local_f1__box2
   CLC
   ADC   g2
   STA   g2
   LDA   __zpabi_f1__p0_0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f0:
   SUBROUTINE

   LDA   #$04
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __zpabi_f0__p0
   LDA   #$3A
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDX   #$00
   LDA   #$11
   JSR   f0
   STA   __local_main__8
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_main__7
   STA   __local_main__6
   STA   __local_main__5
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   #$00
   EOR   g1+2
   STA   __local_main__1
   LDA   #$00
   EOR   g1+3
   STA   __local_main__4
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__4
   STA   __local_main__0
   LDA   __local_main__8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   __local_main__7
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__6
   EOR   __local_main__1
   STA   HARGS+2
   LDA   __local_main__5
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

g0:
   DC.W  $0045

g1:
   DC.L  $0000004A

__zp_init:
   SUBROUTINE

   LDA   #$A8
   STA   g2
   RTS
