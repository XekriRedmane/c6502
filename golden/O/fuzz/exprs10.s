SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v0	EQU	$26
v1	EQU	$27
v3	EQU	$28
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
__local_check__7	EQU	$8B
__local_check__8	EQU	$8C
__local_check__9	EQU	$8D
__local_check__10	EQU	$8E
__local_check__11	EQU	$8F

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14,__local_check__15,__local_check__16 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v2 section=.data local=false zero=false
; item v4 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v0 size=1 at=auto def=true init=$01 weight=5
; zp v1 size=1 at=auto def=true init=$FF weight=5
; zp v3 size=1 at=auto def=true init=$80 weight=3
; runtime hargs_size=32 hargs_need=20
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
   STA   __local_check__1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__0
   LDA   v1
   BMI   .cmp_true@6
   BNE   .cmp_true@6
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_check__fail_1
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   HARGS+6
   STA   HARGS+7
   LDA   v3
   STA   HARGS
   LDA   __local_check__1
   STA   HARGS+1
   LDA   __local_check__0
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__fail_1
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JSR   mul32
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   HARGS+8
   CMP   #$80
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@11:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_check__4
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   AND   #$1F
   STA   __local_check__r
   LDA   __local_check__4
   EOR   #$01
   ORA   v1
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__r
   BEQ   .lnot_true@16
   LDA   #$00
   BEQ   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   STA   HARGS+4
   JSR   check@run0
   JSR   asl32
   LDA   HARGS+8
   BNE   .jcmp_skip@18
   LDA   HARGS+7
   BNE   .jcmp_skip@18
   LDA   HARGS+6
   BNE   .jcmp_skip@18
   LDA   HARGS+5
   CMP   #$0C
   BEQ   .if_end@1
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   v0
   CMP   #$01
   BEQ   .vthread@1
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   v0
   BMI   .sx_neg@19
.vthread@1:
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check__1
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_check__0
   STA   __local_check__r
   LDA   v0
   EOR   v2
   STA   HARGS+4
   LDA   __local_check__1
   EOR   v2+1
   STA   HARGS+5
   LDA   __local_check__0
   EOR   v2+2
   STA   HARGS+6
   LDA   __local_check__r
   EOR   v2+3
   STA   HARGS+7
   JSR   check@run0
   JSR   mul32
   LDA   v3
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_check__1
   LDA   v0
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_check__r
   LDA   v3
   AND   v0
   STA   __local_check__5
   LDA   __local_check__1
   AND   __local_check__r
   STA   __local_check__4
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   STA   __local_check__r
   LDA   #$00
   CMP   __local_check__r
   BNE   .cmp_true@29
   CMP   __local_check__0
   BNE   .cmp_true@29
   CMP   __local_check__1
   BNE   .cmp_true@29
   CMP   __local_check__2
   BNE   .cmp_true@29
   CMP   __local_check__3
   BNE   .cmp_true@29
   CMP   __local_check__4
   BNE   .cmp_true@29
   LDA   #$07
   CMP   __local_check__5
   BNE   .cmp_true@29
   LDA   #$00
   BEQ   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
   STA   __local_check__0
   STA   __local_check__r
   LDA   HARGS+11
   CMP   __local_check__r
   BNE   .if_end@3
   LDA   HARGS+10
   CMP   __local_check__0
   BNE   .if_end@3
   LDA   HARGS+9
   BNE   .if_end@3
   LDA   HARGS+8
   CMP   __local_check__1
   BNE   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v0
   BMI   .sx_neg@34
   LDA   #$00
   BEQ   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   STA   __local_check__4
   BMI   .sx_neg@36
   LDA   #$00
   BEQ   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   STA   __local_check__3
   STA   __local_check__r
   STA   __local_check__2
   STA   __local_check__1
   AND   #$1F
   STA   __local_check__0
   LDA   __local_check__r
   CLC
   ADC   #$80
   STA   __local_check__r
   BCC   .carry_inc@0
   INC   __local_check__2
   BNE   .carry_inc@0
   INC   __local_check__1
   BNE   .carry_inc@0
   INC   __local_check__0
.carry_inc@0:
   LDA   __local_check__4
   STA   __local_check__11
   LDA   __local_check__3
   STA   __local_check__10
   LDA   __local_check__r
   STA   __local_check__9
   LDA   __local_check__2
   STA   __local_check__8
   LDA   __local_check__1
   STA   __local_check__7
   LDA   __local_check__0
   AND   #$1F
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@39
   LDA   #$FF
.sx_done@39:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   AND   #$1F
   STA   __local_check__r
   LDA   __local_check__6
   CMP   __local_check__r
   BNE   .thread@0
   LDA   __local_check__7
   CMP   __local_check__0
   BNE   .thread@0
   LDA   __local_check__8
   CMP   __local_check__1
   BNE   .thread@0
   LDA   __local_check__9
   CMP   __local_check__2
   BNE   .thread@0
   LDA   __local_check__10
   CMP   __local_check__3
   BNE   .thread@0
   LDA   __local_check__11
   BNE   .thread@0
   LDA   v0
   CMP   v1
   BEQ   .cmp_true@40
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   EOR   #$FF
   STA   __local_check__0
   LDA   #$00
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@43
   LDA   __local_check__0
   CMP   #$FF
   BEQ   .if_end@4
.jcmp_skip@43:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   #$00
   BPL   .sx_done@45
   LDA   #$FF
.sx_done@45:
   STA   __local_check__0
   STA   __local_check__r
   LDA   v5
   SEC
   SBC   v4
   STA   __local_check__6
   LDA   v5+1
   SBC   #$00
   STA   __local_check__5
   LDA   v5+2
   SBC   __local_check__0
   STA   __local_check__4
   LDA   v5+3
   SBC   __local_check__r
   STA   __local_check__3
   LDA   v5
   STA   HARGS
   LDA   v5+1
   STA   HARGS+1
   LDA   v5+2
   STA   HARGS+2
   LDA   v5+3
   STA   HARGS+3
   LDA   v0
   STA   HARGS+4
   JSR   lsr32
   LDA   __local_check__6
   SEC
   SBC   HARGS+5
   LDA   __local_check__5
   SBC   HARGS+6
   LDA   __local_check__4
   SBC   HARGS+7
   LDA   __local_check__3
   SBC   HARGS+8
   LDA   #$00
   ROL
   STA   __local_check__4
   LDA   v1
   BMI   .sx_neg@48
   LDA   #$00
   BEQ   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   STA   __local_check__1
   LDA   v3
   BMI   .sx_neg@50
   LDA   #$00
   BEQ   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   STA   __local_check__r
   LDA   v1
   ORA   v3
   STA   __local_check__0
   LDA   __local_check__1
   ORA   __local_check__r
   STA   __local_check__r
   LDA   __local_check__0
   SEC
   SBC   v4
   LDA   __local_check__r
   SBC   #$00
   BVC   .cmp_novf@52
   EOR   #$80
.cmp_novf@52:
   ASL
   LDA   #$00
   ROL
   AND   __local_check__4
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@53
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@53:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   v1
   CMP   #$FF
   BEQ   .if_end@7
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

check@run0:
   SUBROUTINE

   LDA   v2
   STA   HARGS
   LDA   v2+1
   STA   HARGS+1
   LDA   v2+2
   STA   HARGS+2
   LDA   v2+3
   STA   HARGS+3
   RTS

main:
   SUBROUTINE

   JMP   check

v2:
   DC.L  $0000000C

v4:
   DS.B  1

v5:
   DC.L  $00000006

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   v0
   LDA   #$FF
   STA   v1
   LDA   #$80
   STA   v3
   RTS
