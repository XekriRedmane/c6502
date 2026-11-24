SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_3	EQU	$83
__local_check__r_2	EQU	$84
__local_check__r_1	EQU	$85
__local_check__r_0	EQU	$86
__local_check__1	EQU	$88
__local_check__2	EQU	$89
__local_check__3	EQU	$8A
__local_check__4	EQU	$8B

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_check__r_0
   STA   __local_check__r_1
   LDA   v5+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   v4
   ORA   v5
   STA   __local_check__r_2
   LDA   v4+1
   ORA   v5+1
   STA   __local_check__r_3
   LDA   v4+2
   ORA   __local_check__fail_0
   STA   __local_check__fail_0
   LDA   v4+3
   ORA   __local_check__fail_1
   STA   __local_check__fail_1
   LDA   __local_check__r_2
   CLC
   SBC   v1
   LDA   __local_check__r_3
   SBC   #$00
   LDA   __local_check__fail_0
   SBC   __local_check__r_0
   LDA   __local_check__fail_1
   SBC   __local_check__r_1
   LDA   #$00
   ROL
   STA   HARGS+2
   JSR   check@run1
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   BNE   .jcmp_skip@4
   LDA   HARGS+4
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@4:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v0+1
   BNE   .jcmp_skip@5
   LDA   v0
   CMP   #$03
   BEQ   .if_end@2
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   BEQ   .and_false@3
   LDA   #$01
   STA   __local_check__r_2
   LDA   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_check__r_2
.and_end@4:
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__r_2
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   v3
   STA   HARGS+4
   LDA   v3+1
   STA   HARGS+5
   LDA   v3+2
   STA   HARGS+6
   LDA   v3+3
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+12
   STA   __local_check__4
   LDA   HARGS+13
   STA   __local_check__3
   LDA   HARGS+14
   STA   __local_check__2
   LDA   HARGS+15
   STA   __local_check__1
   LDA   v0+1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__r_1
   LDA   v4+3
   BNE   .cmp_true@10
   LDA   v4+2
   BNE   .cmp_true@10
   LDA   v4+1
   BNE   .cmp_true@10
   LDA   v4
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_check__r_3
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   HARGS+6
   STA   HARGS+7
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   __local_check__r_1
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__r_3
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JSR   udivmod32
   LDA   HARGS+8
   STA   HARGS+4
   LDA   HARGS+9
   STA   HARGS+5
   LDA   HARGS+10
   STA   HARGS+6
   LDA   HARGS+11
   STA   HARGS+7
   LDA   __local_check__4
   STA   HARGS
   LDA   __local_check__3
   STA   HARGS+1
   LDA   __local_check__2
   STA   HARGS+2
   LDA   __local_check__1
   JSR   check@run0
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   v0+1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   #$FF
   SEC
   SBC   v0
   LDA   #$FF
   SBC   v0+1
   LDA   #$FF
   SBC   __local_check__r_2
   LDA   #$7F
   SBC   __local_check__r_3
   LDA   #$00
   ROL
   EOR   #$01
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v2+1
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_check__1
   LDA   v0+1
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v4
   CLC
   ADC   v0
   STA   HARGS+4
   LDA   v4+1
   ADC   v0+1
   STA   HARGS+5
   LDA   v4+2
   ADC   __local_check__r_2
   STA   HARGS+6
   LDA   v4+3
   ADC   __local_check__r_3
   STA   HARGS+7
   JSR   check@run1
   LDA   __local_check__1
   STA   HARGS+2
   JSR   check@run0
   BEQ   .vthread@0
   LDA   v0
   ORA   v0+1
   BNE   .if_end@9
.vthread@0:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   v3+3
   BNE   .jcmp_skip@21
   LDA   v3+2
   BNE   .jcmp_skip@21
   LDA   v3+1
   BNE   .jcmp_skip@21
   LDA   v3
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@21:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@1

check@run1:
   SUBROUTINE

   LDA   v2
   STA   HARGS
   LDA   v2+1
   STA   HARGS+1
   RTS

check@run0:
   SUBROUTINE

   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+12
   ORA   HARGS+13
   ORA   HARGS+14
   ORA   HARGS+15
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DC.W  $0003

v1:
   DC.B  $D2

v2:
   DC.W  $0001

v3:
   DC.L  $00000001

v4:
   DC.L  $A93F32A9

v5:
   DC.W  $FFFF
