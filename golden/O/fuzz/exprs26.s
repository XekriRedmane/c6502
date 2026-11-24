SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v3	EQU	$26
v1	EQU	$27
v2	EQU	$28
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86
__local_check__0	EQU	$87

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.bss local=false zero=true
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v3 size=1 at=auto def=true init=$01 weight=6
; zp v1 size=1 at=auto def=true init=$01 weight=3
; zp v2 size=1 at=auto def=true init=$FF weight=3
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__fail_1
   LDA   v3
   SEC
   SBC   v1
   LDA   __local_check__fail_1
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   #$00
   STA   __local_check__fail_0
   LDX   v0
   BEQ   .shift_done@4
.shift_loop@3:
   LDA   __local_check__fail_0
   ASL
   ROR   __local_check__fail_0
   ROR   __local_check__fail_1
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDA   __local_check__fail_1
   ORA   __local_check__fail_0
   BEQ   .and_false@0
   LDA   v2
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__fail_1
   LDA   v5
   SEC
   SBC   v2
   LDA   #$00
   SBC   __local_check__fail_1
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   JSR   check@run1
   STA   __local_check__r_1
   LDA   v2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__r_3
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   v4
   SEC
   SBC   v2
   LDA   v4+1
   SBC   __local_check__r_3
   LDA   v4+2
   SBC   __local_check__fail_0
   LDA   v4+3
   SBC   __local_check__fail_1
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   JSR   check@run1
   STA   __local_check__fail_1
   LDA   __local_check__r_1
   SEC
   SBC   __local_check__fail_1
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@13
   EOR   #$80
.jcmp_novf@13:
   BPL   .check@ssa_block@3
.and_false@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@2
.check@ssa_block@3:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   v1
   CMP   #$01
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   v2
   STA   __local_check__r_0
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_check__r_1
   LDA   v3
   STA   HARGS+1
   LDA   v1
   STA   HARGS+2
   LDA   #$00
   STA   HARGS
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   HARGS+2
   LDA   HARGS+5
   STA   HARGS+3
   LDA   __local_check__r_0
   STA   HARGS
   LDA   __local_check__r_1
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+7
   BNE   .jcmp_skip@18
   LDA   HARGS+6
   CMP   #$FF
   BEQ   .if_end@4
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   v3
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check__r_1
   LDA   v3
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_check__r_3
   LDA   v3
   SEC
   SBC   v3
   LDA   __local_check__r_3
   SBC   __local_check__r_1
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   JSR   check@run1
   STA   __local_check__0
   LDA   v0+3
   CMP   v4+3
   BNE   .cmp_true@24
   LDA   v0+2
   CMP   v4+2
   BNE   .cmp_true@24
   LDA   v0+1
   CMP   v4+1
   BNE   .cmp_true@24
   LDA   v0
   CMP   v4
   BNE   .cmp_true@24
   LDA   #$00
   BEQ   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   STA   __local_check__r_3
   LDA   __local_check__0
   SEC
   SBC   __local_check__r_3
   STA   HARGS
   LDA   #$00
   SBC   #$00
   STA   HARGS+1
   LDA   v3
   AND   #$7F
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   v3
   BEQ   .if_end@9
   JSR   check@run0
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   JSR   check@run0
   BEQ   .if_end@12
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   v4+3
   CMP   #$FF
   BNE   .jcmp_skip@27
   LDA   v4+2
   CMP   #$FF
   BNE   .jcmp_skip@27
   LDA   v4+1
   CMP   #$FF
   BNE   .jcmp_skip@27
   LDA   v4
   CMP   #$FF
   BEQ   .if_end@13
.jcmp_skip@27:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@13:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS

check@run1:
   SUBROUTINE

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

check@run0:
   SUBROUTINE

   LDA   v0
   ORA   v0+1
   ORA   v0+2
   ORA   v0+3
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DS.B  4

v4:
   DC.L  $FFFFFFFF

v5:
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   v1
   STA   v3
   LDA   #$FF
   STA   v2
   RTS
