SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v1	EQU	$26
v3	EQU	$27
v2	EQU	$28
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r	EQU	$83
__local_check__0	EQU	$84
__local_check__1	EQU	$85
__local_check__2	EQU	$86
__local_check__3	EQU	$87
__local_check__4	EQU	$88
__local_check__5	EQU	$89
__local_check__6	EQU	$8A

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v1 size=1 at=auto def=true init=$0F weight=6
; zp v3 size=1 at=auto def=true init=$0C weight=5
; zp v2 size=1 at=auto def=true init=$00 weight=3
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   v0
   AND   v2
   STA   __local_check__4
   LDA   v0+2
   AND   __local_check__fail_0
   STA   __local_check__2
   LDA   v0+3
   AND   __local_check__fail_1
   STA   __local_check__1
   LDA   v5
   ORA   v0
   STA   __local_check__0
   LDA   v5+1
   ORA   v0+1
   STA   __local_check__r
   LDA   __local_check__0
   CLC
   SBC   __local_check__4
   LDA   __local_check__r
   SBC   #$00
   LDA   v0+2
   SBC   __local_check__2
   LDA   v0+3
   SBC   __local_check__1
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   #$00
   BNE   .jcmp_skip@2
   LDA   __local_check__fail_1
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v1
   CMP   #$0F
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDX   v1
   LDA   v5
   STA   __local_check__r
   LDA   v5+1
   STA   __local_check__0
   CPX   #$00
   BEQ   .shift_done@4
.shift_loop@3:
   LSR   __local_check__0
   ROR   __local_check__r
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDA   __local_check__r
   CLC
   SBC   #$02
   LDA   __local_check__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   #$00
   BPL   .if_end@2
   LDA   #$FF
   BNE   .if_end@2
   LDA   #$00
   CMP   #$01
   BNE   .if_end@2
   LDA   __local_check__1
   BNE   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v3
   BMI   .jcmp_skip@9
   CMP   #$0C
   BEQ   .vthread@0
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
   LDA   v3
   BMI   .sx_neg@10
.vthread@0:
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_check__2
   LDA   v3
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_check__0
   LDA   v3
   SEC
   SBC   v2
   LDA   __local_check__0
   SBC   #$00
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   CLC
   ADC   v3
   BCC   .carry_inc@0
   INC   __local_check__2
.carry_inc@0:
   SEC
   SBC   v2
   LDA   __local_check__2
   SBC   #$00
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r
   LDA   #$00
   BNE   .jcmp_skip@16
   LDA   __local_check__r
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@16:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   v5
   ORA   v5+1
   BEQ   .lnot_true@17
   LDA   #$00
   BEQ   .lnot_end@18
.lnot_true@17:
   LDA   #$01
.lnot_end@18:
   SEC
   SBC   v1
   STA   __local_check__0
   LDA   #$00
   SBC   #$00
   CMP   #$FF
   BNE   .jcmp_skip@19
   LDA   __local_check__0
   CMP   #$F2
   BEQ   .if_end@5
.jcmp_skip@19:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   v1
   CMP   #$0F
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   HARGS+2
   STA   HARGS+3
   LDA   v1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   v4
   STA   HARGS+4
   LDA   v4+1
   STA   HARGS+5
   LDA   v4+2
   STA   HARGS+6
   LDA   v4+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   __local_check__6
   LDA   HARGS+9
   STA   __local_check__5
   LDA   HARGS+10
   STA   __local_check__4
   LDA   HARGS+11
   STA   __local_check__3
   LDA   v3
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_check__1
   LDA   v3
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_check__r
   LDA   v3
   SEC
   SBC   v3
   LDA   __local_check__1
   SBC   __local_check__r
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   HARGS
   LDA   #$01
   AND   v1
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_check__0
   STA   __local_check__r
   LDA   __local_check__3
   CMP   __local_check__r
   BNE   .if_end@7
   LDA   __local_check__4
   CMP   __local_check__0
   BNE   .if_end@7
   LDA   __local_check__5
   CMP   HARGS+7
   BNE   .if_end@7
   LDA   __local_check__6
   CMP   HARGS+6
   BNE   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@7:
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
   DC.L  $00000001

v4:
   DC.L  $488B09AC

v5:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v2
   LDA   #$0F
   STA   v1
   LDA   #$0C
   STA   v3
   RTS
