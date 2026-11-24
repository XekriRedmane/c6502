SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v1	EQU	$26
v5	EQU	$28
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_1	EQU	$83
__local_check__r_0	EQU	$84
__local_check__0	EQU	$85
__local_check__1	EQU	$86
__local_check__2	EQU	$87
__local_check__3	EQU	$88
__local_check__4	EQU	$89
__local_check__5	EQU	$8A
__local_check__6	EQU	$8B
__local_check__7	EQU	$8C

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run2 section=.text local=true zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v1 size=2 at=auto def=true init=$00,$80 weight=6
; zp v5 size=1 at=auto def=true init=$FF weight=3
; runtime hargs_size=32 hargs_need=29
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$00
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v3
   CLC
   SBC   #$00
   LDA   v3+1
   SBC   #$00
   LDA   v3+2
   SBC   #$00
   LDA   v3+3
   SBC   #$00
   LDA   v3+4
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__r_1
   SEC
   LDA   #$00
   SBC   __local_check__r_1
   STA   __local_check__0
   LDA   #$00
   SBC   #$00
   STA   __local_check__r_0
   LDA   v1+1
   BNE   .cmp_true@0
   LDA   v1
   CMP   v5
   BNE   .cmp_true@0
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   EOR   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDA   __local_check__0
   STA   HARGS
   LDA   __local_check__r_0
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   v1+1
   CMP   #$80
   BNE   .jcmp_skip@3
   LDA   v1
   BEQ   .if_end@2
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDX   v5
   STX   HARGS
   LDX   #$00
   STX   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   AND   #$01
   STA   HARGS+4
   LDA   v3
   STA   HARGS+8
   LDA   v3+1
   STA   HARGS+9
   LDA   v3+2
   STA   HARGS+10
   LDA   v3+3
   STA   HARGS+11
   LDA   v3+4
   STA   HARGS+12
   JSR   check@run2
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+20
   AND   #$01
   STA   __local_check__r_1
   LDA   #$00
   CLC
   SBC   HARGS+16
   LDA   #$00
   SBC   HARGS+17
   LDA   #$00
   SBC   HARGS+18
   LDA   #$00
   SBC   HARGS+19
   LDA   #$00
   SBC   __local_check__r_1
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__0
   STA   __local_check__r_0
   LDA   __local_check__1
   STA   __local_check__2
   LDA   __local_check__0
   AND   #$01
   STA   __local_check__r_1
   LDA   v3
   SEC
   SBC   __local_check__2
   STA   __local_check__2
   LDA   v3+1
   SBC   #$00
   STA   __local_check__1
   LDA   v3+2
   SBC   __local_check__0
   STA   __local_check__0
   LDA   v3+3
   SBC   __local_check__r_0
   STA   __local_check__r_0
   LDA   v3+4
   SBC   __local_check__r_1
   LSR
   BCS   .jcmp_skip@8
   LDA   __local_check__r_0
   BNE   .jcmp_skip@8
   LDA   __local_check__0
   BNE   .jcmp_skip@8
   LDA   __local_check__1
   BNE   .jcmp_skip@8
   LDA   __local_check__2
   CMP   #$0E
   BEQ   .if_end@3
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v1
   ORA   v3
   STA   __local_check__7
   LDA   v1+1
   ORA   v3+1
   STA   __local_check__6
   LDA   v1
   CLC
   SBC   v1
   LDA   v1+1
   SBC   v1+1
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   __local_check__0
   STA   __local_check__r_0
   LDA   __local_check__1
   STA   __local_check__2
   LDA   __local_check__0
   AND   #$01
   STA   __local_check__r_1
   LDA   __local_check__7
   SEC
   SBC   __local_check__2
   STA   __local_check__2
   LDA   __local_check__6
   SBC   #$00
   STA   __local_check__1
   LDA   v3+2
   SBC   __local_check__0
   STA   __local_check__0
   LDA   v3+3
   SBC   __local_check__r_0
   STA   __local_check__r_0
   LDA   v3+4
   SBC   __local_check__r_1
   LDX   __local_check__2
   STX   __local_check__7
   LDX   __local_check__1
   STX   __local_check__6
   LDX   __local_check__0
   STX   __local_check__5
   LDX   __local_check__r_0
   STX   __local_check__4
   AND   #$01
   STA   __local_check__3
   LDA   v0
   STA   HARGS+8
   LDA   v0+1
   STA   HARGS+9
   LDA   v0+2
   STA   HARGS+10
   LDA   v0+3
   STA   HARGS+11
   JSR   check@run0
   STA   HARGS+12
   JSR   check@run1
   SEC
   LDA   #$00
   SBC   HARGS+24
   STA   __local_check__2
   LDA   #$00
   SBC   HARGS+25
   STA   __local_check__1
   LDA   #$00
   SBC   HARGS+26
   STA   __local_check__0
   LDA   #$00
   SBC   HARGS+27
   STA   __local_check__r_0
   LDA   #$00
   SBC   HARGS+28
   AND   #$01
   STA   __local_check__r_1
   LDA   __local_check__2
   SEC
   SBC   __local_check__7
   LDA   __local_check__1
   SBC   __local_check__6
   LDA   __local_check__0
   SBC   __local_check__5
   LDA   __local_check__r_0
   SBC   __local_check__4
   LDA   __local_check__r_1
   SBC   __local_check__3
   LDA   #$00
   ROL
   STA   __local_check__r_1
   LDA   #$00
   BNE   .jcmp_skip@11
   LDA   __local_check__r_1
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   v1
   ORA   v4
   STA   __local_check__1
   LDA   v1+1
   ORA   v4+1
   STA   __local_check__0
   LDA   __local_check__1
   CLC
   SBC   v3
   LDA   __local_check__0
   SBC   v3+1
   LDA   v4+2
   SBC   v3+2
   LDA   v4+3
   SBC   v3+3
   LDA   #$00
   SBC   v3+4
   LDA   #$00
   ROL
   STA   __local_check__r_1
   LDA   #$00
   SEC
   SBC   __local_check__r_1
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__r_1
   LDA   #$00
   BNE   .jcmp_skip@13
   LDA   __local_check__r_1
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   #$01
   SEC
   SBC   v0
   STA   __local_check__2
   LDA   #$00
   SBC   v0+1
   STA   __local_check__1
   LDA   #$00
   SBC   v0+2
   STA   __local_check__0
   LDA   #$00
   SBC   v0+3
   STA   __local_check__r_0
   LDA   #$00
   SBC   #$00
   AND   #$01
   STA   __local_check__r_1
   LDA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__r_0
   ORA   __local_check__r_1
   BEQ   .lnot_true@14
   LDA   #$00
   BEQ   .lnot_end@15
.lnot_true@14:
   LDA   #$01
.lnot_end@15:
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   LDX   __local_check__1
   STX   __local_check__7
   LDX   #$00
   STX   __local_check__6
   STA   __local_check__5
   STA   __local_check__4
   AND   #$01
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   LDX   v5
   STX   HARGS+8
   LDX   #$00
   STX   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   AND   #$01
   STA   HARGS+12
   JSR   check@run0
   JSR   check@run1
   LDA   HARGS+24
   EOR   #$FF
   STA   __local_check__2
   LDA   HARGS+25
   EOR   #$FF
   STA   __local_check__1
   LDA   HARGS+26
   EOR   #$FF
   STA   __local_check__0
   LDA   HARGS+27
   EOR   #$FF
   STA   __local_check__r_0
   LDA   HARGS+28
   EOR   #$FF
   AND   #$01
   STA   __local_check__r_1
   LDA   __local_check__3
   CMP   __local_check__r_1
   BNE   .cmp_true@20
   LDA   __local_check__4
   CMP   __local_check__r_0
   BNE   .cmp_true@20
   LDA   __local_check__5
   CMP   __local_check__0
   BNE   .cmp_true@20
   LDA   __local_check__6
   CMP   __local_check__1
   BNE   .cmp_true@20
   LDA   __local_check__7
   CMP   __local_check__2
   BNE   .cmp_true@20
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   STA   __local_check__r_1
   LDA   #$00
   BNE   .jcmp_skip@23
   LDA   __local_check__r_1
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@23:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@7:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

check@run2:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   RTS

check@run1:
   SUBROUTINE

   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JMP   udivmod64

check@run0:
   SUBROUTINE

   LDA   v3
   STA   HARGS
   LDA   v3+1
   STA   HARGS+1
   LDA   v3+2
   STA   HARGS+2
   LDA   v3+3
   STA   HARGS+3
   LDA   v3+4
   STA   HARGS+4
   JSR   check@run2
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DC.L  $10000000

v2:
   DC.B  $08

v3:
   DC.B  $0E
   DS.B  4

v4:
   DS.B  4

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v1
   LDA   #$80
   STA   v1+1
   LDA   #$FF
   STA   v5
   RTS
