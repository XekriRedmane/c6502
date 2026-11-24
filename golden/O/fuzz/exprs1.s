SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v3	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_1	EQU	$83
__local_check__r_0	EQU	$84
__local_check__0	EQU	$85
__local_check__1	EQU	$86
__local_check__2	EQU	$87
__local_check__3	EQU	$88
__local_check__4	EQU	$89

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v3 size=1 at=auto def=true init=$FF weight=3
; runtime hargs_size=32 hargs_need=9
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
   STA   __local_check__fail_0
   LDA   v0
   ORA   v3
   STA   __local_check__fail_1
   LDA   __local_check__fail_0
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__fail_1
   CMP   #$FF
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   SEC
   BCS   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   #$FF
   CLC
   ADC   v5
   STA   __local_check__1
   LDA   #$00
   ADC   v5+1
   STA   __local_check__0
   LDA   #$00
   ADC   v5+2
   STA   __local_check__r_0
   LDA   #$00
   ADC   v5+3
   STA   __local_check__r_1
   LDA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__r_0
   ORA   __local_check__r_1
   BEQ   .check@jthread@0
   LDA   v1+1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   HARGS+2
   STA   HARGS+3
   LDA   v1
   STA   HARGS
   LDA   v1+1
   STA   HARGS+1
   LDA   #$08
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   BMI   .if_end@8
.check@jthread@0:
   LDA   v1
   ORA   v1+1
   BNE   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@8:
   LDA   v4
   CLC
   SBC   v4
   LDA   v4+1
   SBC   v4+1
   LDA   #$00
   ROL
   STA   __local_check__4
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __local_check__3
   STA   __local_check__2
   LDA   v1+1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__r_0
   STA   __local_check__r_1
   LDA   __local_check__4
   AND   v1
   STA   __local_check__1
   LDA   #$00
   AND   v1+1
   STA   __local_check__0
   LDA   __local_check__3
   AND   __local_check__r_0
   STA   __local_check__r_0
   LDA   __local_check__2
   AND   __local_check__r_1
   STA   __local_check__r_1
   LDA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__r_0
   ORA   __local_check__r_1
   BEQ   .if_end@13
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@13:
   LDA   v0
   SEC
   SBC   v2
   LDA   #$00
   SBC   v2+1
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   v1+1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_check__r_0
   STA   __local_check__r_1
   LDA   v5
   SEC
   SBC   v1
   LDA   v5+1
   SBC   v1+1
   LDA   v5+2
   SBC   __local_check__r_0
   LDA   v5+3
   SBC   __local_check__r_1
   LDA   #$00
   ROL
   CLC
   ADC   __local_check__2
   STA   __local_check__r_0
   LDA   #$00
   ADC   #$00
   STA   __local_check__r_1
   SEC
   LDA   #$00
   SBC   __local_check__r_0
   STA   __local_check__r_0
   LDA   #$00
   SBC   __local_check__r_1
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__r_0
   CMP   #$FF
   BEQ   .if_end@14
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@14:
   LDA   v5
   ORA   v5+1
   ORA   v5+2
   ORA   v5+3
   BEQ   .if_end@15
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@15:
   LDA   v2+1
   BNE   .jcmp_skip@14
   LDA   v2
   CMP   #$06
   BEQ   .if_end@17
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
   LDA   v2+1
   BNE   .jcmp_skip@15
   LDA   v2
   CMP   #$06
   BEQ   .if_end@17
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@17:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

main:
   SUBROUTINE

   JMP   check

v0:
   DC.B  $91

v1:
   DC.W  $FFFF

v2:
   DC.W  $0006

v4:
   DC.W  $9B81

v5:
   DS.B  4

__zp_init:
   SUBROUTINE

   LDA   #$FF
   STA   v3
   RTS
