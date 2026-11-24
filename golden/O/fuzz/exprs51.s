SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v0	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86
__local_check__0	EQU	$87
__local_check__1	EQU	$88
__local_check__2	EQU	$89

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.bss local=false zero=true
; item v4 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v0 size=1 at=auto def=true init=$01 weight=3
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v5+1
   BNE   .thread@0
   LDA   v5
   CMP   v0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   STA   HARGS
   SEC
   LDA   #$00
   SBC   v0
   STA   HARGS+2
   LDA   #$00
   SBC   #$00
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   v0
   AND   HARGS+4
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   CLC
   SBC   #$02
   LDA   #$01
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__r_1
   JSR   check@run0
   BNE   .or_true@1
   LDA   #$00
   STA   __local_check__r_2
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   __local_check__r_2
.or_end@2:
   LDA   __local_check__r_1
   SEC
   SBC   __local_check__r_2
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   STA   __local_check__0
   LDA   v2
   CLC
   ADC   #$FF
   STA   __local_check__r_0
   LDA   v2+1
   ADC   #$FF
   STA   __local_check__r_1
   LDA   v2+2
   ADC   #$00
   STA   __local_check__r_2
   LDA   v2+3
   ADC   #$00
   BNE   .cmp_true@3
   LDA   __local_check__r_2
   BNE   .cmp_true@3
   LDA   __local_check__r_1
   BNE   .cmp_true@3
   LDA   __local_check__r_0
   CMP   #$01
   BNE   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   ORA   __local_check__1
   STA   __local_check__r_2
   LDA   __local_check__0
   BNE   .jcmp_skip@6
   LDA   __local_check__r_2
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   v2
   STA   HARGS
   LDA   v2+1
   STA   HARGS+1
   LDA   v2+2
   STA   HARGS+2
   LDA   v2+3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+4
   LDY   #$01
   STY   HARGS+5
   DEY
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   #$7F
   CLC
   ADC   v5
   STA   __local_check__r_0
   LDA   #$00
   ADC   v5+1
   STA   __local_check__r_1
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   HARGS+12
   AND   __local_check__r_0
   STA   __local_check__2
   LDA   HARGS+13
   AND   __local_check__r_1
   STA   __local_check__1
   LDA   HARGS+14
   AND   __local_check__r_2
   STA   __local_check__0
   LDA   HARGS+15
   AND   __local_check__r_3
   STA   __local_check__r_0
   LDA   #$00
   CMP   v3+3
   BNE   .cmp_end@10
   CMP   v3+2
   BNE   .cmp_end@10
   LDA   #$23
   CMP   v3+1
   BNE   .thread@1
   LDA   #$45
   CMP   v3
   BEQ   .cmp_true@9
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_check__r_1
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   #$FF
   SEC
   SBC   __local_check__r_1
   LDA   #$FF
   SBC   #$00
   LDA   #$FF
   SBC   __local_check__r_2
   LDA   #$FF
   SBC   __local_check__r_3
   LDA   #$00
   ROL
   STA   __local_check__r_1
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   __local_check__r_1
   SEC
   SBC   __local_check__2
   LDA   #$00
   SBC   __local_check__1
   LDA   __local_check__r_2
   SBC   __local_check__0
   LDA   __local_check__r_3
   SBC   __local_check__r_0
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r_3
   LDA   #$00
   BNE   .jcmp_skip@17
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@17:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   BEQ   .vthread@0
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   #$00
.vthread@0:
   SEC
   SBC   v3
   STA   __local_check__r_0
   LDA   v3
   SBC   v3+1
   STA   __local_check__r_1
   LDA   v3+1
   SBC   v3+2
   STA   __local_check__r_2
   LDA   v3+2
   SBC   v3+3
   STA   __local_check__r_3
   LDA   __local_check__r_0
   ORA   __local_check__r_1
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .lnot_true@18
   LDA   #$00
   BEQ   .lnot_end@19
.lnot_true@18:
   LDA   #$01
.lnot_end@19:
   STA   __local_check__r_2
   LDA   #$00
   BNE   .jcmp_skip@20
   LDA   __local_check__r_2
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@20:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   JSR   check@run0
   BEQ   .if_end@13
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
   JMP   .if_end@0

check@run0:
   SUBROUTINE

   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   RTS

main:
   SUBROUTINE

   JMP   check

v1:
   DC.B  $0D

v2:
   DC.L  $FFFFFFFF

v3:
   DS.B  4

v4:
   DS.B  4

v5:
   DC.W  $8000

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   v0
   RTS
