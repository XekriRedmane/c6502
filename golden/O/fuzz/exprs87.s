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
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.bss local=false zero=true
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v0 size=2 at=auto def=true init=$01,$00 weight=7
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v2
   ORA   v2+1
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v4
   STA   HARGS
   LDA   v4+1
   STA   HARGS+1
   LDA   v4+2
   STA   HARGS+2
   LDA   v4+3
   STA   HARGS+3
   LDA   v0
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   HARGS+4
   LDA   HARGS+6
   STA   HARGS+5
   LDA   HARGS+7
   STA   HARGS+6
   LDA   HARGS+8
   STA   HARGS+7
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   v2+1
   AND   #$07
   EOR   #$04
   SEC
   SBC   #$04
   STA   __local_check__2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__1
   STA   __local_check__0
   LDA   v2
   SEC
   SBC   v4
   LDA   __local_check__2
   SBC   v4+1
   LDA   __local_check__1
   SBC   v4+2
   LDA   __local_check__0
   SBC   v4+3
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   HARGS+4
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   JSR   asl32
   LDA   HARGS+5
   ORA   HARGS+6
   ORA   HARGS+7
   ORA   HARGS+8
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   v1
   CMP   #$80
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v4+3
   CMP   #$62
   BNE   .jcmp_skip@3
   LDA   v4+2
   CMP   #$AA
   BNE   .jcmp_skip@3
   LDA   v4+1
   CMP   #$C2
   BNE   .jcmp_skip@3
   LDA   v4
   CMP   #$C9
   BEQ   .if_end@3
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   #$01
   EOR   v2
   STA   __local_check__1
   LDA   #$00
   EOR   v2+1
   STA   __local_check__0
   LDA   v0+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v0
   SEC
   SBC   #$02
   LDA   v0+1
   SBC   #$00
   LDA   __local_check__r_2
   SBC   #$00
   LDA   __local_check__r_3
   SBC   #$00
   LDA   #$00
   ROL
   ORA   __local_check__1
   STA   __local_check__r_2
   LDA   __local_check__0
   BNE   .jcmp_skip@6
   LDA   __local_check__r_2
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   v0
   ORA   #$02
   STA   HARGS+2
   LDA   v0+1
   STA   HARGS+3
   STA   HARGS+1
   LDA   v0
   STA   HARGS
   JSR   sdivmod16
   LDA   HARGS+5
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   HARGS+4
   ORA   HARGS+5
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   v5+1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v4
   EOR   v5
   STA   __local_check__r_0
   LDA   v4+1
   EOR   v5+1
   STA   __local_check__r_1
   LDA   v4+2
   EOR   __local_check__r_2
   STA   __local_check__r_2
   LDA   v4+3
   EOR   __local_check__r_3
   STA   __local_check__r_3
   LDA   __local_check__r_0
   ORA   __local_check__r_1
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .lnot_true@11
   LDA   #$00
   BEQ   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   STA   __local_check__r_2
   LDY   #$00
   TYA
   EOR   #$04
   SEC
   SBC   #$04
   ORA   __local_check__r_2
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   v2
   EOR   #$98
   STA   __local_check__r_0
   LDA   v0
   ORA   v0
   STA   __local_check__r_2
   LDA   v0+1
   ORA   v0+1
   STA   __local_check__r_3
   LDA   v2+1
   CMP   __local_check__r_3
   BNE   .cmp_true@13
   LDA   __local_check__r_0
   CMP   __local_check__r_2
   BNE   .cmp_true@13
   TYA
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_check__r_1
   LDA   v2+1
   BNE   .or_true@7
   LDA   v2
   CMP   v1
   BNE   .or_true@7
   LDA   v0
   ORA   v0+1
   BNE   .or_true@7
   LDA   #$00
   STA   __local_check__r_2
   JMP   .vthread@0
.or_true@7:
   LDA   #$01
   STA   __local_check__r_2
   TYA
   BNE   .thread@0
.vthread@0:
   LDA   __local_check__r_1
   CMP   __local_check__r_2
   BEQ   .cmp_true@16
.thread@0:
   TYA
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __local_check__r_3
   TYA
   BNE   .jcmp_skip@19
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@19:
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

main:
   SUBROUTINE

   JMP   check

v1:
   DC.B  $80

v2:
   DS.B  2

v3:
   DC.B  $E5

v4:
   DC.L  $62AAC2C9

v5:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v0+1
   LDA   #$01
   STA   v0
   RTS
