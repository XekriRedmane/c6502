SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v2	EQU	$26
v3	EQU	$27
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r	EQU	$83
__local_check__0	EQU	$84
__local_check__1	EQU	$85
__local_check__2	EQU	$86
__local_check__3	EQU	$87

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r,__local_check__0,__local_check__1,__local_check__2,__local_check__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v2 size=1 at=auto def=true init=$80 weight=5
; zp v3 size=1 at=auto def=true init=$00 weight=4
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v3
   ORA   v2
   STA   __local_check__fail_1
   LDA   v1
   EOR   __local_check__fail_1
   STA   __local_check__r
   LDA   v1+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   AND   #$1F
   CMP   #$1F
   BNE   .jcmp_skip@2
   LDA   v1+1
   CMP   #$80
   BNE   .jcmp_skip@2
   LDA   __local_check__r
   CMP   #$80
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v2
   CMP   #$80
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v0
   BNE   .or_true@2
   LDA   v3
   BNE   .or_true@2
   LDA   #$00
   STA   __local_check__1
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   __local_check__1
   LDA   #$00
.or_end@3:
   ORA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   AND   #$1F
   STA   __local_check__r
   LDA   v3
   SEC
   SBC   __local_check__1
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   __local_check__r
   LDA   #$00
   ROL
   CLC
   ORA   #$00
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@6:
   LDA   #$07
   SBC   v5
   LDA   #$00
   SBC   v5+1
   LDA   #$00
   SBC   v5+2
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   v0
   BEQ   .and_false@8
   LDA   v4
   ORA   v4+1
   ORA   v4+2
   BEQ   .and_false@8
   LDX   #$01
   BNE   .and_end@9
.and_false@8:
   LDX   #$00
.and_end@9:
   LDA   #$00
   STA   __local_check__3
   CPX   #$00
   BEQ   .shift_done@6
.shift_loop@5:
   ASL   __local_check__2
   ROL   __local_check__3
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   __local_check__3
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDX   __local_check__2
   STX   __local_check__1
   LDX   __local_check__3
   STX   __local_check__0
   AND   #$03
   STA   __local_check__r
   LDA   __local_check__1
   SEC
   SBC   v5
   LDA   __local_check__0
   SBC   v5+1
   LDA   __local_check__r
   SBC   v5+2
   LDA   #$00
   ROL
   STA   __local_check__r
   LDA   #$00
   BNE   .jcmp_skip@9
   LDA   __local_check__r
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@10:
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   LDX   v2
   STX   HARGS
   LDX   #$00
   STX   HARGS+1
   AND   #$03
   JSR   check@run0
   STA   HARGS+4
   LDY   #$80
   STY   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   JSR   check@run1
   LDA   HARGS+10
   JSR   check@run0
   LDY   #$07
   STY   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+10
   AND   #$03
   STA   __local_check__0
   LDA   #$7F
   BNE   .cmp_true@12
   LDA   #$FF
   CMP   v3
   BNE   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   HARGS+4
   JSR   check@run1
   LDA   __local_check__0
   JSR   check@run0
   JSR   lsr32
   LDA   HARGS+5
   ORA   HARGS+6
   ORA   HARGS+7
   BEQ   .if_end@11
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@11:
   LDA   v2
   CMP   #$80
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

check@run1:
   SUBROUTINE

   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   RTS

check@run0:
   SUBROUTINE

   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DC.B  $0F

v1:
   DC.W  $8000

v4:
   DC.B  $FF
   DC.B  $FF
   DC.B  $1F

v5:
   DS.B  3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v3
   LDA   #$80
   STA   v2
   RTS
