SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v1	EQU	$26
v4	EQU	$27
v0	EQU	$28
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r	EQU	$83
__local_check__0	EQU	$84
__local_check__1	EQU	$85
__local_check__2	EQU	$86
__local_check__3	EQU	$87
__local_check__5	EQU	$89
__local_check__6	EQU	$8A

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v1 size=1 at=auto def=true init=$FF weight=7
; zp v4 size=1 at=auto def=true init=$FF weight=4
; zp v0 size=2 at=auto def=true init=$76,$E4 weight=7
; runtime hargs_size=32 hargs_need=11
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
   STA   __local_check__fail_1
   LDA   v2
   ORA   v4
   STA   __local_check__r
   LDA   v2+1
   ORA   __local_check__fail_1
   STA   __local_check__fail_0
   LDA   v2
   LSR
   LDA   #$00
   ROR
   STA   __local_check__fail_1
   LDA   __local_check__fail_0
   CMP   __local_check__fail_1
   BEQ   .lb_skip@1
   JMP   .check@asm_ssa_split@0
.lb_skip@1:
   LDA   __local_check__r
   BEQ   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v0
   STA   HARGS+2
   LDA   #$07
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   v0+1
   STA   HARGS+3
   JSR   udivmod16
   LDA   #$01
   AND   v3
   STA   __local_check__r
   LDA   HARGS+4
   EOR   __local_check__r
   STA   __local_check__0
   LDA   HARGS+5
   BNE   .jcmp_skip@5
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   v1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_check__r
   LDA   #$FF
   SEC
   SBC   v1
   STA   __local_check__2
   LDA   #$FF
   SBC   __local_check__r
   STA   __local_check__1
   LDA   v1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__r
   LDA   __local_check__1
   CMP   __local_check__r
   BNE   .thread@0
   LDA   __local_check__2
   CMP   v1
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_check__2
   LDA   v1
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__0
   LDA   v0+1
   CMP   v0+1
   BNE   .thread@1
   LDA   v0
   CMP   v0
   BEQ   .cmp_true@15
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   STA   __local_check__r
   LDA   v1
   SEC
   SBC   __local_check__r
   LDA   __local_check__0
   SBC   #$00
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__r
   LDA   __local_check__2
   SEC
   SBC   __local_check__r
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@19
   EOR   #$80
.jcmp_novf@19:
   BMI   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v1
   CMP   #$FF
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v0
   CLC
   ADC   #$FF
   STA   __local_check__2
   LDA   v0+1
   ADC   #$FF
   STA   __local_check__1
   LDA   v2
   STA   __local_check__r
   LDA   v2+1
   STA   __local_check__0
   LDX   v2
   BEQ   .shift_done@21
.shift_loop@20:
   ASL   __local_check__r
   ROL   __local_check__0
   DEX
   BNE   .shift_loop@20
.shift_done@21:
   LDA   __local_check__r
   CLC
   SBC   __local_check__2
   LDA   __local_check__0
   SBC   __local_check__1
   LDA   #$00
   ROL
   LSR
   BCC   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   v2
   STA   HARGS+2
   LDA   v2+1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+7
   CMP   v0+1
   BNE   .thread@2
   LDA   HARGS+6
   CMP   v0
   BEQ   .cmp_true@22
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   EOR   v0
   STA   __local_check__0
   LDA   #$00
   EOR   v0+1
   CMP   #$E4
   BNE   .jcmp_skip@25
   LDA   __local_check__0
   CMP   #$76
   BEQ   .if_end@7
.jcmp_skip@25:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   v4
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_check__1
   LDA   v1
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __local_check__r
   SEC
   LDA   #$00
   SBC   v1
   STA   __local_check__0
   LDA   #$00
   SBC   __local_check__r
   STA   __local_check__r
   LDA   v4
   SEC
   SBC   __local_check__0
   LDA   __local_check__1
   SBC   __local_check__r
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   JSR   check@run0
   STA   __local_check__3
   LDA   v4
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   HARGS+1
   LDA   v4
   STA   HARGS
   LDA   #$DC
   STA   HARGS+2
   LDA   #$74
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __local_check__0
   STA   __local_check__r
   LDA   v3
   CLC
   SBC   HARGS+6
   LDA   v3+1
   SBC   HARGS+7
   LDA   v3+2
   SBC   __local_check__0
   LDA   v3+3
   SBC   __local_check__r
   LDA   #$00
   ROL
   SEC
   SBC   __local_check__3
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@35
   EOR   #$80
.cmp_novf@35:
   JSR   check@run0
   STA   __local_check__r
   LDA   #$00
   BNE   .jcmp_skip@36
   LDA   __local_check__r
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@36:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   v1
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   __local_check__1
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_check__0
   STA   __local_check__r
   LDA   v1
   SEC
   SBC   v3
   LDA   __local_check__1
   SBC   v3+1
   LDA   __local_check__0
   SBC   v3+2
   LDA   __local_check__r
   SBC   v3+3
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@42
   LDA   #$FF
.sx_done@42:
   CMP   v3+3
   BNE   .cmp_true@43
   CMP   v3+2
   BNE   .cmp_true@43
   LDA   #$00
   CMP   v3+1
   BNE   .cmp_true@43
   LDA   __local_check__1
   CMP   v3
   BNE   .cmp_true@43
   LDA   #$00
   BEQ   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   STA   __local_check__6
   LDA   v5
   BMI   .sx_neg@46
   LDA   #$00
   BEQ   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   STA   __local_check__1
   LDA   v1
   BMI   .sx_neg@48
   LDA   #$00
   BEQ   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   STA   __local_check__r
   LDA   v1
   SEC
   SBC   v5
   LDA   __local_check__r
   SBC   __local_check__1
   BVC   .cmp_novf@50
   EOR   #$80
.cmp_novf@50:
   JSR   check@run0
   STA   __local_check__5
   LDA   #$00
   BPL   .sx_done@52
   LDA   #$FF
.sx_done@52:
   STA   __local_check__3
   LDA   v4
   BMI   .sx_neg@53
   LDA   #$00
   BEQ   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   STA   __local_check__1
   BMI   .sx_neg@55
   LDA   #$00
   BEQ   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   STA   __local_check__0
   STA   __local_check__r
   LDA   #$00
   AND   v4
   STA   __local_check__2
   LDA   #$80
   AND   __local_check__1
   STA   __local_check__1
   LDA   #$00
   AND   __local_check__0
   STA   __local_check__0
   LDA   #$00
   AND   __local_check__r
   STA   __local_check__r
   LDA   __local_check__3
   CMP   __local_check__r
   BNE   .thread@3
   CMP   __local_check__0
   BNE   .thread@3
   LDA   #$00
   CMP   __local_check__1
   BNE   .cmp_end@58
   LDA   __local_check__5
   CMP   __local_check__2
   BEQ   .cmp_true@57
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   ORA   __local_check__6
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@60
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@60:
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

check@run0:
   SUBROUTINE

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

main:
   SUBROUTINE

   JMP   check

v2:
   DC.W  $0005

v3:
   DC.L  $FFFFFFFF

v5:
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$76
   STA   v0
   LDA   #$E4
   STA   v0+1
   LDA   #$FF
   STA   v1
   STA   v4
   RTS
