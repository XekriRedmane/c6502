SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v2	EQU	$26
v4	EQU	$27
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r	EQU	$83
__local_check__0	EQU	$84
__local_check__1	EQU	$85
__local_check__2	EQU	$86
__local_check__3	EQU	$87
__local_check__4	EQU	$88

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v2 size=1 at=auto def=true init=$01 weight=3
; zp v4 size=1 at=auto def=true init=$04 weight=3
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDA   v2
   STA   HARGS+4
   JSR   mul32
   LDA   v0
   STA   __local_check__fail_1
   LDA   v0+1
   STA   __local_check__fail_0
   LDX   v1
   BEQ   .shift_done@1
.shift_loop@0:
   ASL   __local_check__fail_1
   ROL   __local_check__fail_0
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDA   __local_check__fail_1
   CLC
   SBC   v0
   LDA   __local_check__fail_0
   SBC   v0+1
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   HARGS+6
   STA   HARGS+7
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   LDA   __local_check__fail_1
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JSR   mul32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v3
   ORA   v3+1
   BEQ   .lnot_true@4
   LDA   #$00
   BEQ   .lnot_end@5
.lnot_true@4:
   LDA   #$01
.lnot_end@5:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__r
   ORA   __local_check__2
   ORA   __local_check__r
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v2
   CMP   #$01
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v0+1
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   v0
   CMP   #$FF
   BEQ   .if_end@3
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v4
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__r
   LDA   v4
   SEC
   SBC   #$01
   LDA   __local_check__r
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__r
   LDX   v4
   LDA   #$00
   STA   __local_check__0
   CPX   #$00
   BEQ   .shift_done@14
.shift_loop@13:
   ASL   __local_check__r
   ROL   __local_check__0
   DEX
   BNE   .shift_loop@13
.shift_done@14:
   LDA   __local_check__0
   BNE   .jcmp_skip@15
   LDA   __local_check__r
   CMP   #$10
   BEQ   .if_end@4
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   v5
   BEQ   .lnot_true@16
   LDA   #$00
   BEQ   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   STA   __local_check__0
   LDA   #$00
   CMP   v3+1
   BNE   .cmp_end@19
   CMP   v3
   BNE   .cmp_end@19
   LDA   #$01
.cmp_end@19:
   STA   __local_check__r
   LDA   __local_check__0
   EOR   __local_check__r
   BEQ   .vthread@0
   LDA   v2
   BNE   .if_end@8
.vthread@0:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   v4
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_check__1
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_check__0
   STA   __local_check__r
   LDA   v4
   CLC
   ADC   #$A0
   STA   __local_check__4
   LDA   __local_check__1
   ADC   #$09
   STA   __local_check__3
   LDA   __local_check__0
   ADC   #$FB
   STA   __local_check__2
   LDA   __local_check__r
   ADC   #$08
   STA   __local_check__1
   LDA   v5
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   HARGS+3
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   v5
   STA   HARGS+2
   JSR   mul16
   LDA   __local_check__1
   BNE   .cmp_true@27
   LDA   __local_check__2
   BNE   .cmp_true@27
   LDA   __local_check__3
   CMP   HARGS+5
   BNE   .cmp_true@27
   LDA   __local_check__4
   CMP   HARGS+4
   BNE   .cmp_true@27
   LDA   #$00
   BEQ   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   STA   __local_check__r
   SEC
   LDA   #$00
   SBC   __local_check__r
   STA   __local_check__0
   LDA   #$00
   SBC   #$00
   CMP   #$FF
   BNE   .jcmp_skip@30
   LDA   __local_check__0
   CMP   #$FF
   BEQ   .if_end@9
.jcmp_skip@30:
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
   DC.W  $FFFF

v1:
   DC.W  $0001

v3:
   DC.W  $0009

v5:
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   v2
   LDA   #$04
   STA   v4
   RTS
