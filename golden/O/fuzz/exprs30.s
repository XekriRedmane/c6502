SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v5	EQU	$26
v1	EQU	$27
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v5 size=1 at=auto def=true init=$01 weight=4
; zp v1 size=1 at=auto def=true init=$0A weight=3
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v2+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   HARGS+6
   STA   HARGS+7
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   v0+2
   STA   HARGS+2
   LDA   v0+3
   STA   HARGS+3
   LDA   v2
   STA   HARGS+4
   LDA   v2+1
   STA   HARGS+5
   JSR   sdivmod32
   LDX   HARGS+12
   LDA   #$00
   STA   __local_check__fail_1
   CPX   #$00
   BEQ   .shift_done@3
.shift_loop@2:
   LSR   __local_check__fail_1
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDA   v2
   ORA   v2+1
   BEQ   .and_false@0
   LDA   v5
   BEQ   .and_false@0
   LDX   #$01
   BNE   .and_end@1
.and_false@0:
   LDX   #$00
.and_end@1:
   BEQ   .shift_done@5
.shift_loop@4:
   LSR   __local_check__fail_1
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDA   __local_check__fail_1
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   v3+3
   CMP   #$22
   BNE   .jcmp_skip@6
   LDA   v3+2
   CMP   #$52
   BNE   .jcmp_skip@6
   LDA   v3+1
   CMP   #$7D
   BNE   .jcmp_skip@6
   LDA   v3
   CMP   #$BD
   BEQ   .if_end@3
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   v5
   AND   #$02
   BEQ   .if_end@8
   SEC
   LDA   v3+1
   SBC   #$80
   LDA   v3+2
   SBC   #$00
   LDA   v3+3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__r_3
   LDA   #$7F
   AND   v5
   TAX
   LDA   #$00
   STA   __local_check__r_2
   CPX   #$00
   BEQ   .shift_done@8
.shift_loop@7:
   LDA   __local_check__r_2
   ASL
   ROR   __local_check__r_2
   ROR   __local_check__r_3
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   LDA   __local_check__r_3
   ORA   __local_check__r_2
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@8:
   LDA   v1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__r_3
   LDA   v1
   CLC
   SBC   #$00
   LDA   __local_check__r_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__r_0
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v5
   CLC
   SBC   v3
   LDA   #$00
   SBC   v3+1
   LDA   __local_check__r_2
   SBC   v3+2
   LDA   __local_check__r_3
   SBC   v3+3
   LDA   #$00
   ROL
   CLC
   ADC   __local_check__r_0
   STA   __local_check__r_0
   LDA   #$00
   ADC   #$00
   STA   __local_check__r_1
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   __local_check__r_0
   CLC
   ADC   v3
   STA   __local_check__r_0
   LDA   __local_check__r_1
   ADC   v3+1
   STA   __local_check__r_1
   LDA   __local_check__r_2
   ADC   v3+2
   STA   __local_check__r_2
   LDA   __local_check__r_3
   ADC   v3+3
   CMP   #$22
   BNE   .jcmp_skip@15
   LDA   __local_check__r_2
   CMP   #$52
   BNE   .jcmp_skip@15
   LDA   __local_check__r_1
   CMP   #$7D
   BNE   .jcmp_skip@15
   LDA   __local_check__r_0
   CMP   #$BE
   BEQ   .if_end@10
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@10:
   LDA   v0+3
   BNE   .jcmp_skip@16
   LDA   v0+2
   BNE   .jcmp_skip@16
   LDA   v0+1
   BNE   .jcmp_skip@16
   LDA   v0
   CMP   #$01
   BEQ   .if_end@11
.jcmp_skip@16:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@11:
   LDA   v3+3
   BNE   .if_end@12
   LDA   v3+2
   BNE   .if_end@12
   LDA   v3+1
   BNE   .if_end@12
   LDA   v3
   CMP   #$80
   BEQ   .if_end@12
   LDA   #$00
   BEQ   .if_end@12
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   v1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   HARGS+1
   LDA   v1
   STA   HARGS
   LDA   v4
   STA   HARGS+2
   LDA   v4+1
   STA   HARGS+3
   JSR   udivmod16
   LDA   v1
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_check__r_3
   LDA   v1
   CLC
   SBC   HARGS+6
   LDA   __local_check__r_3
   SBC   HARGS+7
   LDA   #$00
   ROL
   STA   __local_check__r_1
   SEC
   LDA   #$00
   SBC   v2
   STA   __local_check__r_2
   LDA   #$00
   SBC   v2+1
   STA   __local_check__r_3
   LDA   __local_check__r_1
   SEC
   SBC   __local_check__r_2
   LDA   #$00
   SBC   __local_check__r_3
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r_3
   LDA   #$00
   BNE   .jcmp_skip@27
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@13
.jcmp_skip@27:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@13:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@2

main:
   SUBROUTINE

   JMP   check

v0:
   DC.L  $00000001

v2:
   DC.W  $FFFF

v3:
   DC.L  $22527DBD

v4:
   DC.W  $0001

__zp_init:
   SUBROUTINE

   LDA   #$0A
   STA   v1
   LDA   #$01
   STA   v5
   RTS
