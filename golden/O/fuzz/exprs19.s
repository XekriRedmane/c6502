SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v0	EQU	$26
v5	EQU	$27
v3	EQU	$28
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86
__local_check__0	EQU	$87
__local_check__1	EQU	$88
__local_check__2	EQU	$89
__local_check__3	EQU	$8A

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v0 size=1 at=auto def=true init=$01 weight=4
; zp v5 size=1 at=auto def=true init=$80 weight=4
; zp v3 size=2 at=auto def=true init=$51,$98 weight=7
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v0
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v5
   STA   HARGS+2
   JSR   check@run1
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+5
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+4
   CMP   #$31
   BEQ   .if_end@1
.jcmp_skip@0:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   v3
   SEC
   SBC   v5
   STA   HARGS
   LDA   v3+1
   SBC   #$00
   JSR   check@run0
   LDA   HARGS+4
   STA   __local_check__r_0
   LDA   HARGS+5
   STA   __local_check__r_1
   LDA   v0
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   HARGS+3
   LDA   v5
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   v0
   STA   HARGS+2
   JSR   sdivmod16
   LDA   HARGS+6
   STA   HARGS
   LDA   HARGS+7
   JSR   check@run0
   LDA   __local_check__r_0
   ORA   HARGS+4
   STA   __local_check__r_2
   LDA   __local_check__r_1
   ORA   HARGS+5
   BNE   .jcmp_skip@3
   LDA   __local_check__r_2
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v3+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v3
   SEC
   SBC   v2
   STA   __local_check__r_0
   LDA   v3+1
   SBC   v2+1
   STA   __local_check__r_1
   LDA   __local_check__r_2
   SBC   v2+2
   STA   __local_check__r_2
   LDA   __local_check__r_3
   SBC   v2+3
   LDX   __local_check__r_0
   STX   __local_check__3
   LDX   __local_check__r_1
   STX   __local_check__2
   LDX   __local_check__r_2
   STX   __local_check__1
   AND   #$7F
   STA   __local_check__0
   LDA   v0
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   HARGS+1
   LDA   v1
   STA   HARGS+2
   LDA   v0
   STA   HARGS
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__r_2
   AND   #$7F
   STA   __local_check__r_3
   LDA   __local_check__3
   ORA   HARGS+4
   STA   __local_check__r_0
   LDA   __local_check__2
   ORA   HARGS+5
   STA   __local_check__r_1
   LDA   __local_check__1
   ORA   __local_check__r_2
   STA   __local_check__r_2
   LDA   __local_check__0
   ORA   __local_check__r_3
   CMP   #$7F
   BNE   .jcmp_skip@10
   LDA   __local_check__r_2
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   __local_check__r_1
   CMP   #$98
   BNE   .jcmp_skip@10
   LDA   __local_check__r_0
   CMP   #$FF
   BEQ   .if_end@3
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v3+1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   HARGS+2
   STA   HARGS+3
   JSR   check@run1
   LDA   #$45
   STA   HARGS+4
   LDA   #$23
   STA   HARGS+5
   LDA   #$01
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   STA   __local_check__3
   LDA   HARGS+9
   STA   __local_check__2
   LDA   HARGS+10
   STA   __local_check__1
   LDA   HARGS+11
   STA   __local_check__0
   LDA   v4
   STA   HARGS
   LDA   v4+1
   STA   HARGS+1
   LDA   v4+2
   STA   HARGS+2
   LDA   v4+3
   STA   HARGS+3
   LDA   v2
   STA   HARGS+4
   LDA   v2+1
   STA   HARGS+5
   LDA   v2+2
   STA   HARGS+6
   LDA   v2+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   CLC
   SBC   __local_check__3
   LDA   HARGS+9
   SBC   __local_check__2
   LDA   HARGS+10
   SBC   __local_check__1
   LDA   HARGS+11
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__r_1
   LDA   v0
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   HARGS+1
   LDA   v0
   STA   HARGS
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   mul16
   LDA   __local_check__r_1
   SEC
   SBC   HARGS+4
   STA   __local_check__r_2
   LDA   #$00
   SBC   HARGS+5
   ORA   __local_check__r_2
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   #$00
   CMP   v4+3
   BNE   .if_end@7
   CMP   v4+2
   BNE   .if_end@7
   CMP   v4+1
   BNE   .if_end@7
   LDA   #$07
   CMP   v4
   BNE   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   v4+3
   CMP   #$80
   BNE   .jcmp_skip@15
   LDA   v4+2
   BNE   .jcmp_skip@15
   LDA   v4+1
   BNE   .jcmp_skip@15
   LDA   v4
   BEQ   .if_end@8
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   v3+1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_check__r_2
   AND   #$7F
   STA   __local_check__r_3
   LDA   v3
   SEC
   SBC   v2
   STA   __local_check__r_0
   LDA   v3+1
   SBC   v2+1
   STA   __local_check__r_1
   LDA   __local_check__r_2
   SBC   v2+2
   STA   __local_check__r_2
   LDA   __local_check__r_3
   SBC   v2+3
   CMP   #$FF
   BNE   .jcmp_skip@18
   LDA   __local_check__r_2
   CMP   #$FF
   BNE   .jcmp_skip@18
   LDA   __local_check__r_1
   CMP   #$98
   BNE   .jcmp_skip@18
   LDA   __local_check__r_0
   CMP   #$51
   BEQ   .if_end@9
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@9:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

check@run1:
   SUBROUTINE

   LDA   v3
   STA   HARGS
   LDA   v3+1
   STA   HARGS+1
   RTS

check@run0:
   SUBROUTINE

   STA   HARGS+1
   LDA   v3
   STA   HARGS+2
   LDA   v3+1
   STA   HARGS+3
   JMP   sdivmod16

main:
   SUBROUTINE

   JMP   check

v1:
   DC.B  $FF

v2:
   DC.L  $80000000

v4:
   DC.L  $80000000

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   v0
   LDA   #$51
   STA   v3
   LDA   #$98
   STA   v3+1
   LDA   #$80
   STA   v5
   RTS
