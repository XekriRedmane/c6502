SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v2	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_0	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__0	EQU	$86
__local_check__1	EQU	$87
__local_check__2	EQU	$88
__local_check__3	EQU	$89
__local_check__4	EQU	$8A
__local_check__5	EQU	$8B
__local_check__6	EQU	$8C

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_0,__local_check__r_2,__local_check__r_1,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v2 size=1 at=auto def=true init=$0D weight=4
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v3+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   HARGS+6
   STA   HARGS+7
   LDA   v0
   STA   HARGS
   JSR   check@run0
   LDA   v3
   STA   HARGS+4
   LDA   v3+1
   STA   HARGS+5
   JSR   sdivmod32
   LDA   HARGS+12
   ORA   HARGS+13
   ORA   HARGS+14
   ORA   HARGS+15
   BNE   .or_true@0
   LDA   v0
   EOR   #$FF
   STA   __local_check__r_2
   LDA   v0+1
   EOR   #$FF
   STA   __local_check__r_0
   LDA   v0+2
   EOR   #$FF
   STA   __local_check__fail_0
   LDA   v0+3
   EOR   #$FF
   STA   __local_check__fail_1
   LDA   __local_check__r_2
   ORA   __local_check__r_0
   ORA   __local_check__fail_0
   ORA   __local_check__fail_1
   BNE   .or_true@0
   LDA   #$00
   STA   __local_check__0
   STA   __local_check__r_1
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_check__0
   LDA   #$00
   STA   __local_check__r_1
.or_end@1:
   LDA   v4
   SEC
   SBC   #$80
   LDA   v4+1
   SBC   #$00
   LDA   v4+2
   SBC   #$00
   LDA   v4+3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__fail_0
   LDA   v2
   STA   HARGS
   LDA   v5
   STA   HARGS+1
   JSR   udivmod8
   LDA   #$00
   BNE   .cmp_true@2
   LDA   __local_check__fail_0
   CMP   HARGS+2
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   ORA   __local_check__0
   STA   __local_check__fail_0
   LDA   __local_check__r_1
   BNE   .jcmp_skip@5
   LDA   __local_check__fail_0
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@5:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   v4+3
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   v4+2
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   v4+1
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   v4
   CMP   #$FF
   BEQ   .if_end@5
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@5:
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __local_check__r_2
   STA   __local_check__r_0
   LDA   v4
   ORA   v2
   STA   __local_check__0
   LDA   v4+2
   ORA   __local_check__r_2
   STA   __local_check__r_2
   LDA   v4+3
   ORA   __local_check__r_0
   CMP   v4+3
   BNE   .cmp_true@9
   LDA   __local_check__r_2
   CMP   v4+2
   BNE   .cmp_true@9
   LDA   v4+1
   CMP   v4+1
   BNE   .cmp_true@9
   LDA   __local_check__0
   CMP   v4
   BNE   .cmp_true@9
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
   STA   __local_check__r_0
   ORA   __local_check__r_1
   ORA   __local_check__r_0
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v0+3
   CMP   #$83
   BNE   .jcmp_skip@14
   LDA   v0+2
   CMP   #$E9
   BNE   .jcmp_skip@14
   LDA   v0+1
   CMP   #$60
   BNE   .jcmp_skip@14
   LDA   v0
   CMP   #$E4
   BEQ   .if_end@7
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   __local_check__r_2
   STA   __local_check__r_0
   LDA   v0+3
   CMP   __local_check__r_0
   BNE   .cmp_true@17
   LDA   v0+2
   CMP   __local_check__r_2
   BNE   .cmp_true@17
   LDA   v0+1
   BNE   .cmp_true@17
   LDA   v0
   CMP   v2
   BNE   .cmp_true@17
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   __local_check__2
   STA   __local_check__1
   JSR   check@run0
   LDA   v0
   STA   HARGS
   LDA   #$FF
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   __local_check__3
   CLC
   ADC   HARGS+8
   STA   __local_check__6
   LDA   #$00
   ADC   HARGS+9
   STA   __local_check__5
   LDA   __local_check__2
   ADC   HARGS+10
   STA   __local_check__4
   LDA   __local_check__1
   ADC   HARGS+11
   STA   __local_check__3
   LDA   v5
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_check__1
   LDA   v1+1
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_check__r_2
   STA   __local_check__r_0
   LDA   v0
   SEC
   SBC   v1
   LDA   v0+1
   SBC   v1+1
   LDA   v0+2
   SBC   __local_check__r_2
   LDA   v0+3
   SBC   __local_check__r_0
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   ORA   v5
   STA   __local_check__r_2
   LDA   __local_check__1
   STA   __local_check__r_0
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_check__6
   STA   HARGS
   LDA   __local_check__5
   STA   HARGS+1
   LDA   __local_check__4
   STA   HARGS+2
   LDA   __local_check__3
   STA   HARGS+3
   LDA   __local_check__r_2
   STA   HARGS+4
   LDA   __local_check__r_0
   STA   HARGS+5
   JSR   udivmod32
   LDA   HARGS+11
   BNE   .jcmp_skip@29
   LDA   HARGS+10
   CMP   #$84
   BNE   .jcmp_skip@29
   LDA   HARGS+9
   CMP   #$6D
   BNE   .jcmp_skip@29
   LDA   HARGS+8
   CMP   #$CF
   BEQ   .if_end@8
.jcmp_skip@29:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   v2
   CMP   #$0D
   BEQ   .if_end@9
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
   JMP   .if_end@2

check@run0:
   SUBROUTINE

   LDA   v0+1
   STA   HARGS+1
   LDA   v0+2
   STA   HARGS+2
   LDA   v0+3
   STA   HARGS+3
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DC.L  $83E960E4

v1:
   DC.W  $8000

v3:
   DC.W  $FFFF

v4:
   DC.L  $FFFFFFFF

v5:
   DC.B  $01

__zp_init:
   SUBROUTINE

   LDA   #$0D
   STA   v2
   RTS
