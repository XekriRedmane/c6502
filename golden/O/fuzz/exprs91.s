SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v0	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__0	EQU	$83
__local_check__1	EQU	$84
__local_check__2	EQU	$85
__local_check__3	EQU	$86

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v0 size=1 at=auto def=true init=$00 weight=4
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v2
   ORA   v4
   STA   __local_check__1
   LDA   v2+1
   ORA   v4+1
   STA   __local_check__0
   LDA   v2+2
   ORA   v4+2
   STA   __local_check__fail_0
   JSR   check@run0
   BEQ   .and_false@0
   LDA   v5
   ORA   v5+1
   ORA   v5+2
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_check__fail_1
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_check__fail_1
.and_end@1:
   LDA   __local_check__1
   STA   HARGS
   LDA   __local_check__0
   STA   HARGS+1
   LDA   __local_check__fail_0
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDA   __local_check__fail_1
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   STA   HARGS
   LDA   HARGS+6
   STA   HARGS+1
   LDA   HARGS+7
   STA   HARGS+2
   LDA   v4
   STA   HARGS+4
   LDA   v4+1
   STA   HARGS+5
   LDA   v4+2
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+14
   BNE   .jcmp_skip@0
   LDA   HARGS+13
   BNE   .jcmp_skip@0
   LDA   HARGS+12
   CMP   #$09
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   v2
   EOR   v5
   STA   __local_check__2
   LDA   v2+1
   EOR   v5+1
   STA   __local_check__1
   LDA   v2+2
   EOR   v5+2
   STA   __local_check__0
   LDA   __local_check__2
   ORA   v4
   STA   __local_check__2
   LDA   __local_check__1
   ORA   v4+1
   STA   __local_check__1
   LDA   __local_check__0
   ORA   v4+2
   STA   __local_check__0
   LDA   __local_check__2
   SEC
   SBC   #$11
   LDA   __local_check__1
   SBC   #$04
   LDA   __local_check__0
   SBC   #$C2
   LDA   #$00
   SBC   #$28
   LDA   #$00
   SBC   #$91
   LDA   #$00
   SBC   #$00
   BCC   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   JSR   check@run0
   BEQ   .lnot_true@1
   LDA   #$00
   BEQ   .lnot_end@2
.lnot_true@1:
   LDA   #$01
.lnot_end@2:
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   AND   #$07
   CMP   v2+2
   BNE   .thread@0
   LDA   #$00
   CMP   v2+1
   BNE   .cmp_end@6
   LDA   v0
   CMP   v2
   BEQ   .cmp_true@5
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@8
   EOR   #$80
.jcmp_novf@8:
   BPL   .if_end@6
   LDA   v2
   ORA   #$BB
   STA   __local_check__2
   LDA   v2+1
   ORA   #$DC
   STA   __local_check__1
   LDA   v2+2
   ORA   #$1E
   STA   __local_check__0
   LDA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@6:
   LDA   v4
   STA   HARGS
   LDA   v4+1
   STA   HARGS+1
   LDA   v4+2
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDA   v0
   STA   __local_check__3
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+7
   AND   #$07
   STA   __local_check__0
   LDA   #$00
   CLC
   SBC   HARGS+5
   LDA   #$00
   SBC   HARGS+6
   LDA   #$00
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __local_check__0
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   ORA   __local_check__1
   BEQ   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@7:
   LDA   v1
   ORA   v1
   STA   __local_check__1
   LDA   v1+1
   ORA   v1+1
   BNE   .cmp_true@9
   LDA   __local_check__1
   CMP   #$02
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@12
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@13
.jcmp_skip@12:
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

check@run0:
   SUBROUTINE

   LDA   v4
   ORA   v4+1
   ORA   v4+2
   RTS

main:
   SUBROUTINE

   JMP   check

v1:
   DC.W  $FFFF

v2:
   DS.B  2
   DC.B  $04

v3:
   DC.W  $8000

v4:
   DC.B  $0F
   DS.B  2

v5:
   DC.B  $F2
   DC.B  $26
   DC.B  $18

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v0
   RTS
