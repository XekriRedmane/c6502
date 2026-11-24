SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v5	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r	EQU	$83
__local_check__0	EQU	$84
__local_check__1	EQU	$85
__local_check__2	EQU	$86
__local_check__3	EQU	$87
__local_check__4	EQU	$88
__local_check__5	EQU	$89

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false
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
; zp v5 size=1 at=auto def=true init=$00 weight=4
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__fail_1
   LDA   v2
   EOR   #$FF
   STA   __local_check__fail_0
   LDA   __local_check__fail_1
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__fail_0
   CMP   #$84
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v3
   SEC
   SBC   #$07
   LDA   v3+1
   SBC   #$00
   BCC   .and_false@2
   LDA   v3+1
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__0
   STA   __local_check__r
   LDA   v4
   SBC   v3
   STA   __local_check__2
   LDA   v4+1
   SBC   v3+1
   STA   __local_check__1
   LDA   v4+2
   SBC   __local_check__0
   STA   __local_check__0
   LDA   v4+3
   SBC   __local_check__r
   STA   __local_check__r
   LDA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__r
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_check__2
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_check__2
.and_end@3:
   ORA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   BNE   .jcmp_skip@7
   LDA   #$00
   BNE   .jcmp_skip@7
   LDA   __local_check__2
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   v3+1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__3
   LDA   v5
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_check__1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_check__0
   STA   __local_check__r
   LDA   __local_check__3
   CMP   __local_check__r
   BNE   .if_end@5
   CMP   __local_check__0
   BNE   .if_end@5
   LDA   v3+1
   CMP   __local_check__1
   BNE   .if_end@5
   LDA   v3
   CMP   v5
   BNE   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   v5
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_check__r
   LDA   v0
   SEC
   SBC   v5
   LDA   v0+1
   SBC   __local_check__r
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __local_check__0
   STA   __local_check__r
   LDA   __local_check__1
   ORA   v4
   STA   __local_check__2
   LDA   __local_check__0
   ORA   v4+2
   STA   __local_check__0
   LDA   __local_check__r
   ORA   v4+3
   BNE   .jcmp_skip@19
   LDA   __local_check__0
   BNE   .jcmp_skip@19
   LDA   v4+1
   BNE   .jcmp_skip@19
   LDA   __local_check__2
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@19:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v5
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __local_check__1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_check__0
   STA   __local_check__r
   LDA   v4
   SEC
   SBC   v5
   LDA   v4+1
   SBC   __local_check__1
   LDA   v4+2
   SBC   __local_check__0
   LDA   v4+3
   SBC   __local_check__r
   LDA   #$00
   ROL
   STA   __local_check__5
   LDA   #$00
   BPL   .sx_done@25
   LDA   #$FF
.sx_done@25:
   STA   __local_check__4
   STA   __local_check__3
   LDA   v1
   STA   __local_check__r
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_check__0
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __local_check__1
   ASL   __local_check__r
   ROL   __local_check__0
   ROL   __local_check__1
   ROL
   STA   __local_check__2
   LDA   __local_check__5
   CLC
   ADC   __local_check__r
   STA   HARGS+4
   LDA   #$00
   ADC   __local_check__0
   STA   HARGS+5
   LDA   __local_check__4
   ADC   __local_check__1
   STA   HARGS+6
   LDA   __local_check__3
   ADC   __local_check__2
   STA   HARGS+7
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+12
   ORA   HARGS+13
   ORA   HARGS+14
   ORA   HARGS+15
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   v2
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   __local_check__r
   LDA   v2
   EOR   #$FF
   STA   __local_check__2
   LDA   __local_check__r
   EOR   #$FF
   STA   __local_check__1
   LDA   v0
   ORA   v0+1
   BNE   .or_true@10
   LDA   #$00
   STA   __local_check__0
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   STA   __local_check__0
.or_end@11:
   LDA   __local_check__2
   SEC
   SBC   __local_check__0
   STA   __local_check__2
   BCS   .carry_inc@1
   DEC   __local_check__1
.carry_inc@1:
   LDA   v5
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   __local_check__0
   LDA   v5
   CLC
   ADC   #$7F
   STA   __local_check__r
   BCC   .carry_inc@0
   INC   __local_check__0
.carry_inc@0:
   LDA   __local_check__2
   CLC
   SBC   __local_check__r
   LDA   __local_check__1
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__r
   LDA   #$00
   BNE   .jcmp_skip@34
   LDA   __local_check__r
   CMP   #$01
   BEQ   .if_end@12
.jcmp_skip@34:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   v1
   CMP   #$4D
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
   JMP   .if_end@1

main:
   SUBROUTINE

   JMP   check

v0:
   DC.W  $8000

v1:
   DC.B  $4D

v2:
   DC.B  $7B

v3:
   DC.W  $8000

v4:
   DC.L  $00000001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v5
   RTS
