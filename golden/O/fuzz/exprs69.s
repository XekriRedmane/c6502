SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v5	EQU	$26
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

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v5 size=1 at=auto def=true init=$FF weight=4
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v1
   CMP   #$80
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v5
   CMP   #$FF
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v4+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$EA
   ORA   v0
   STA   __local_check__r_0
   LDA   #$A1
   ORA   v0+1
   STA   __local_check__r_1
   LDA   #$40
   ORA   v0+2
   STA   __local_check__r_2
   LDA   #$5F
   ORA   v0+3
   STA   __local_check__r_3
   LDA   v4
   CLC
   ADC   __local_check__r_0
   STA   __local_check__r_0
   LDA   v4+1
   ADC   __local_check__r_1
   STA   __local_check__r_1
   LDA   __local_check__1
   ADC   __local_check__r_2
   STA   __local_check__r_2
   LDA   __local_check__0
   ADC   __local_check__r_3
   BNE   .cmp_true@2
   LDA   __local_check__r_2
   BNE   .cmp_true@2
   LDA   __local_check__r_1
   CMP   #$26
   BNE   .cmp_true@2
   LDA   __local_check__r_0
   CMP   #$86
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   STA   __local_check__r_3
   LDA   #$00
   BNE   .jcmp_skip@5
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v5
   STA   __local_check__r_2
   LDA   #$00
   STA   __local_check__r_1
   LDX   v3
   BEQ   .shift_done@7
.shift_loop@6:
   ASL   __local_check__r_2
   ROL   __local_check__r_1
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   __local_check__r_1
   BNE   .if_end@3
   LDA   __local_check__r_2
   CMP   v5
   BNE   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v4
   SEC
   SBC   v4
   LDA   v4+1
   SBC   v4+1
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r_1
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v3
   SEC
   SBC   __local_check__r_1
   STA   __local_check__r_0
   LDA   v3+1
   SBC   #$00
   STA   __local_check__r_1
   LDA   v3+2
   SBC   __local_check__r_2
   STA   __local_check__r_2
   LDA   v3+3
   SBC   __local_check__r_3
   STA   __local_check__r_3
   SEC
   LDA   #$00
   SBC   __local_check__r_0
   STA   __local_check__r_0
   LDA   #$00
   SBC   __local_check__r_1
   STA   __local_check__r_1
   LDA   #$00
   SBC   __local_check__r_2
   STA   __local_check__r_2
   LDA   #$00
   SBC   __local_check__r_3
   STA   __local_check__r_3
   LDA   __local_check__r_0
   ORA   __local_check__r_1
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   v0+3
   BNE   .jcmp_skip@14
   LDA   v0+2
   BNE   .jcmp_skip@14
   LDA   v0+1
   BNE   .jcmp_skip@14
   LDA   v0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   v4+1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v4
   CLC
   ADC   v2
   STA   __local_check__r_0
   LDA   v4+1
   ADC   v2+1
   STA   __local_check__r_1
   LDA   __local_check__r_2
   ADC   v2+2
   STA   __local_check__r_2
   LDA   __local_check__r_3
   ADC   v2+3
   STA   __local_check__r_3
   LDA   __local_check__r_0
   ORA   __local_check__r_1
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BNE   .vthread@0
   LDA   v4
   ORA   v4+1
   BNE   .vthread@0
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.vthread@0:
   LDA   #$FF
   STA   __local_check__3
   STA   __local_check__2
   BMI   .sx_neg@22
   LDA   #$00
.sx_neg@22:
   STA   __local_check__1
   STA   __local_check__0
   LDA   v3
   STA   HARGS
   LDA   v3+1
   STA   HARGS+1
   LDA   v3+2
   STA   HARGS+2
   LDA   v3+3
   STA   HARGS+3
   LDA   v2
   STA   HARGS+4
   LDA   v2+1
   STA   HARGS+5
   LDA   v2+2
   STA   HARGS+6
   LDA   v2+3
   STA   HARGS+7
   JSR   sdivmod32
   SEC
   LDA   #$00
   SBC   HARGS+12
   STA   __local_check__r_0
   LDA   #$00
   SBC   HARGS+13
   STA   __local_check__r_1
   LDA   #$00
   SBC   HARGS+14
   STA   __local_check__r_2
   LDA   #$00
   SBC   HARGS+15
   STA   __local_check__r_3
   LDA   __local_check__r_0
   SEC
   SBC   __local_check__3
   LDA   __local_check__r_1
   SBC   __local_check__2
   LDA   __local_check__r_2
   SBC   __local_check__1
   LDA   __local_check__r_3
   SBC   __local_check__0
   BVC   .jcmp_novf@24
   EOR   #$80
.jcmp_novf@24:
   BPL   .if_end@9
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
   DC.L  $00000001

v1:
   DC.B  $80

v2:
   DC.L  $FFFFFFFF

v3:
   DC.L  $00000001

v4:
   DC.W  $FFFF

__zp_init:
   SUBROUTINE

   LDA   #$FF
   STA   v5
   RTS
