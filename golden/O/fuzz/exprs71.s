SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v4	EQU	$26
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
__local_check__4	EQU	$8B
__local_check__5	EQU	$8C

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v4 size=1 at=auto def=true init=$01 weight=5
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v4
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__r_3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   v4
   SEC
   SBC   v2
   LDA   __local_check__r_3
   SBC   v2+1
   LDA   __local_check__fail_0
   SBC   v2+2
   LDA   __local_check__fail_1
   SBC   v2+3
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BMI   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v1
   STA   HARGS+4
   LDA   v1+1
   STA   HARGS+5
   LDA   v1+2
   STA   HARGS+6
   LDA   v5
   STA   HARGS
   LDA   v5+1
   STA   HARGS+1
   LDA   v5+2
   STA   HARGS+2
   LDA   v5+3
   STA   HARGS+3
   LDA   v1+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+15
   CMP   v5+3
   BNE   .thread@0
   LDA   HARGS+14
   CMP   v5+2
   BNE   .thread@0
   LDA   HARGS+13
   CMP   v5+1
   BNE   .thread@0
   LDA   HARGS+12
   CMP   v5
   BEQ   .cmp_true@5
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   __local_check__r_1
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   __local_check__r_1
   ORA   v1
   STA   __local_check__r_0
   LDA   __local_check__r_2
   ORA   v1+2
   STA   __local_check__r_2
   LDA   __local_check__r_3
   ORA   v1+3
   BNE   .jcmp_skip@10
   LDA   __local_check__r_2
   BNE   .jcmp_skip@10
   LDA   v1+1
   BNE   .jcmp_skip@10
   LDA   __local_check__r_0
   CMP   #$02
   BEQ   .if_end@1
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   v4
   LSR
   LDA   #$00
   ROR
   SEC
   SBC   v4
   STA   __local_check__5
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_check__4
   LDA   v4
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__r_1
   LDA   v4
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_check__r_3
   LDA   v4
   SEC
   SBC   v4
   STA   __local_check__3
   LDA   __local_check__r_1
   SBC   __local_check__r_3
   STA   __local_check__2
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_check__1
   STA   __local_check__0
   LDA   v2
   ORA   v2
   STA   __local_check__r_0
   LDA   v2+1
   ORA   v2+1
   STA   __local_check__r_1
   LDA   v2+2
   ORA   v2+2
   STA   __local_check__r_2
   LDA   v2+3
   ORA   v2+3
   STA   __local_check__r_3
   LDA   __local_check__3
   SEC
   SBC   __local_check__r_0
   LDA   __local_check__2
   SBC   __local_check__r_1
   LDA   __local_check__1
   SBC   __local_check__r_2
   LDA   __local_check__0
   SBC   __local_check__r_3
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__r_3
   LDA   __local_check__4
   BNE   .cmp_true@20
   LDA   __local_check__5
   CMP   __local_check__r_3
   BNE   .cmp_true@20
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   STA   __local_check__r_3
   LDA   #$00
   BNE   .jcmp_skip@23
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@23:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v5
   STA   HARGS
   STA   HARGS+4
   LDA   v5+1
   STA   HARGS+1
   STA   HARGS+5
   LDA   v5+2
   STA   HARGS+2
   STA   HARGS+6
   LDA   v5+3
   STA   HARGS+3
   STA   HARGS+7
   JSR   mul32
   LDA   #$01
   AND   HARGS+8
   STA   HARGS+4
   LDA   v1
   STA   HARGS
   LDA   v1+1
   STA   HARGS+1
   LDA   v1+2
   STA   HARGS+2
   LDA   v1+3
   STA   HARGS+3
   JSR   asl32
   LDA   HARGS+8
   BNE   .jcmp_skip@24
   LDA   HARGS+7
   BNE   .jcmp_skip@24
   LDA   HARGS+6
   BNE   .jcmp_skip@24
   LDA   HARGS+5
   CMP   #$04
   BEQ   .if_end@5
.jcmp_skip@24:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   SEC
   LDA   #$00
   SBC   v5
   STA   __local_check__r_2
   LDA   #$00
   SBC   v5+1
   BNE   .jcmp_skip@25
   LDA   __local_check__r_2
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@25:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v3
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_check__r_1
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v0
   SEC
   SBC   v3
   STA   __local_check__r_0
   LDA   v0+1
   SBC   __local_check__r_1
   STA   __local_check__r_1
   LDA   v0+2
   SBC   __local_check__r_2
   STA   __local_check__r_2
   LDA   v0+3
   SBC   __local_check__r_3
   STA   __local_check__r_3
   LDA   __local_check__r_0
   EOR   #$FF
   STA   __local_check__r_0
   LDA   __local_check__r_1
   EOR   #$FF
   STA   __local_check__r_1
   LDA   __local_check__r_2
   EOR   #$FF
   STA   __local_check__r_2
   LDA   __local_check__r_3
   EOR   #$FF
   STA   __local_check__r_3
   LDA   __local_check__r_0
   SEC
   SBC   v1
   LDA   __local_check__r_1
   SBC   v1+1
   LDA   __local_check__r_2
   SBC   v1+2
   LDA   __local_check__r_3
   SBC   v1+3
   LDA   #$00
   ROL
   STA   __local_check__r_3
   LDA   #$00
   BNE   .jcmp_skip@30
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@30:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   v1+3
   BNE   .jcmp_skip@31
   LDA   v1+2
   BNE   .jcmp_skip@31
   LDA   v1+1
   BNE   .jcmp_skip@31
   LDA   v1
   CMP   #$02
   BEQ   .if_end@8
.jcmp_skip@31:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   #$BA
   STA   HARGS
   LDA   #$DC
   STA   HARGS+1
   LDA   #$FE
   STA   HARGS+2
   LDA   #$FF
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
   LDA   v5
   SEC
   SBC   v1
   STA   __local_check__r_0
   LDA   v5+1
   SBC   v1+1
   STA   __local_check__r_1
   LDA   v5+2
   SBC   #$00
   STA   __local_check__r_2
   LDA   v5+3
   SBC   #$00
   STA   __local_check__r_3
   LDA   HARGS+11
   CMP   __local_check__r_3
   BNE   .cmp_true@32
   LDA   HARGS+10
   CMP   __local_check__r_2
   BNE   .cmp_true@32
   LDA   HARGS+9
   CMP   __local_check__r_1
   BNE   .cmp_true@32
   LDA   HARGS+8
   CMP   __local_check__r_0
   BNE   .cmp_true@32
   LDA   #$00
   BEQ   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   STA   __local_check__r_3
   LDA   #$00
   BNE   .jcmp_skip@35
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@35:
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

v0:
   DC.L  $822A87DC

v1:
   DC.L  $00000002

v2:
   DC.L  $00000001

v3:
   DC.B  $01

v5:
   DC.L  $FFFFFFFF

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   v4
   RTS
