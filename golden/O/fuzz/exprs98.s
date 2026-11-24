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
__local_check__0	EQU	$87
__local_check__1	EQU	$88
__local_check__2	EQU	$89
__local_check__3	EQU	$8A
__local_check__4	EQU	$8B
__local_check__5	EQU	$8C
__local_check__6	EQU	$8D

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.bss local=false zero=true
; item v3 section=.data local=false zero=false
; item v4 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=25
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v4
   ORA   v4+1
   ORA   v4+2
   ORA   v4+3
   ORA   v4+4
   ORA   v4+5
   BEQ   .check@jthread@0
   LDA   v1
   ORA   v1+1
   ORA   v1+2
   ORA   v1+3
   BEQ   .lb_skip@1
   JMP   .check@asm_ssa_split@0
.lb_skip@1:
.check@jthread@0:
   LDA   v1
   SEC
   SBC   v1
   LDA   v1+1
   SBC   v1+1
   LDA   v1+2
   SBC   v1+2
   LDA   v1+3
   SBC   v1+3
   BCC   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   v0+3
   ORA   #$80
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_check__r_0
   STA   __local_check__r_1
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v0
   SEC
   SBC   v4
   LDA   v0+1
   SBC   #$00
   LDA   v0+2
   SBC   __local_check__r_0
   LDA   __local_check__3
   SBC   __local_check__r_1
   LDA   #$00
   SBC   __local_check__r_2
   LDA   #$00
   SBC   __local_check__r_3
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__r_3
   LDA   #$00
   CMP   v3+6
   BNE   .cmp_true@3
   CMP   v3+5
   BNE   .cmp_true@3
   CMP   v3+4
   BNE   .cmp_true@3
   LDA   v5+3
   CMP   v3+3
   BNE   .cmp_true@3
   LDA   v5+2
   CMP   v3+2
   BNE   .cmp_true@3
   LDA   v5+1
   CMP   v3+1
   BNE   .cmp_true@3
   LDA   v5
   CMP   v3
   BNE   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_check__r_0
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   HARGS+2
   STA   HARGS+3
   STA   HARGS+4
   LDX   __local_check__r_0
   STX   HARGS
   LDX   #$00
   STX   HARGS+1
   STA   HARGS+5
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   LDA   #$D4
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   ASL
   TYA
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDX   HARGS+24
   LDA   #$00
   STA   __local_check__r_2
   CPX   #$00
   BEQ   .shift_done@9
.shift_loop@8:
   ASL   __local_check__r_3
   ROL   __local_check__r_2
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   __local_check__r_2
   BNE   .jcmp_skip@10
   LDA   __local_check__r_3
   CMP   #$02
   BEQ   .if_end@5
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@5:
   LDA   v4
   EOR   #$FF
   STA   __local_check__6
   LDA   v4+1
   EOR   #$FF
   STA   __local_check__5
   LDA   v4+2
   EOR   #$FF
   STA   __local_check__4
   LDA   v4+3
   EOR   #$FF
   STA   __local_check__3
   LDA   v4+4
   EOR   #$FF
   STA   __local_check__2
   LDA   v4+5
   EOR   #$FF
   STA   __local_check__1
   LDA   v5
   CLC
   ADC   v1
   STA   __local_check__r_0
   LDA   v5+1
   ADC   v1+1
   STA   __local_check__r_1
   LDA   v5+2
   ADC   v1+2
   STA   __local_check__r_2
   LDA   v5+3
   ADC   v1+3
   STA   __local_check__r_3
   LDA   #$00
   CLC
   SBC   __local_check__r_0
   LDA   #$00
   SBC   __local_check__r_1
   LDA   #$00
   SBC   __local_check__r_2
   LDA   #$00
   SBC   __local_check__r_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_check__r_0
   STA   __local_check__r_1
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   __local_check__6
   SEC
   SBC   __local_check__0
   STA   __local_check__0
   LDA   __local_check__5
   SBC   #$00
   STA   __local_check__5
   LDA   __local_check__4
   SBC   __local_check__r_0
   STA   __local_check__r_0
   LDA   __local_check__3
   SBC   __local_check__r_1
   STA   __local_check__r_1
   LDA   __local_check__2
   SBC   __local_check__r_2
   STA   __local_check__r_2
   LDA   __local_check__1
   SBC   __local_check__r_3
   LDX   __local_check__0
   STX   __local_check__1
   LDX   __local_check__5
   STX   __local_check__0
   AND   #$0F
   EOR   #$08
   SEC
   SBC   #$08
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__r_2
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__r_1
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__r_0
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   __local_check__1
   CMP   #$FF
   BEQ   .if_end@6
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@6:
   JSR   check@run0
   LDA   v0
   STA   HARGS+4
   LDA   v0+1
   STA   HARGS+5
   LDA   v0+2
   STA   HARGS+6
   LDA   v0+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   CLC
   STA   HARGS+4
   LDA   HARGS+9
   STA   HARGS+5
   LDA   HARGS+10
   STA   HARGS+6
   LDA   HARGS+11
   ADC   #$00
   STA   HARGS+7
   JSR   check@run0
   JSR   mul32
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@14
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@14
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@14
   LDA   HARGS+8
   CMP   #$FF
   BEQ   .if_end@7
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@7:
   LDA   v2
   ORA   v2+1
   ORA   v2+2
   ORA   v2+3
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   v4+5
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   AND   #$0F
   STA   __local_check__r_3
   LDA   v3
   SEC
   SBC   v4
   STA   __local_check__2
   LDA   v3+1
   SBC   v4+1
   STA   __local_check__1
   LDA   v3+2
   SBC   v4+2
   STA   __local_check__0
   LDA   v3+3
   SBC   v4+3
   STA   __local_check__r_0
   LDA   v3+4
   SBC   v4+4
   STA   __local_check__r_1
   LDA   v3+5
   SBC   v4+5
   STA   __local_check__r_2
   LDA   v3+6
   SBC   __local_check__r_3
   AND   #$0F
   STA   __local_check__r_3
   LDA   __local_check__2
   SEC
   SBC   #$FF
   LDA   __local_check__1
   SBC   #$FF
   LDA   __local_check__0
   SBC   #$00
   LDA   __local_check__r_0
   SBC   #$00
   LDA   __local_check__r_1
   SBC   #$00
   LDA   __local_check__r_2
   SBC   #$00
   LDA   __local_check__r_3
   SBC   #$00
   LDA   #$00
   STA   __local_check__r_2
   ROL
   ASL
   ROL   __local_check__r_2
   ASL
   ROL   __local_check__r_2
   ORA   __local_check__r_2
   BEQ   .if_end@10
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   v2+3
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   #$7D
   SEC
   SBC   v2
   LDA   #$D3
   SBC   v2+1
   LDA   #$49
   SBC   v2+2
   LDA   #$90
   SBC   v2+3
   LDA   #$8D
   SBC   __local_check__r_2
   LDA   #$03
   SBC   __local_check__r_3
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   HARGS+2
   STA   HARGS+3
   STA   HARGS+4
   STA   HARGS+5
   STA   HARGS+6
   LDA   v5
   STA   HARGS+8
   LDA   v5+1
   STA   HARGS+9
   LDA   v5+2
   STA   HARGS+10
   LDA   v5+3
   STA   HARGS+11
   LDA   __local_check__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+7
   STA   HARGS+12
   LDY   #$00
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   v3
   AND   HARGS+16
   STA   __local_check__2
   LDA   v3+1
   AND   HARGS+17
   STA   __local_check__1
   LDA   v3+2
   AND   HARGS+18
   STA   __local_check__0
   LDA   v3+3
   AND   HARGS+19
   STA   __local_check__r_0
   LDA   v3+4
   AND   HARGS+20
   STA   __local_check__r_1
   LDA   v3+5
   AND   HARGS+21
   STA   __local_check__r_2
   LDA   v3+6
   AND   HARGS+22
   STA   __local_check__r_3
   LDA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__r_0
   ORA   __local_check__r_1
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .if_end@11
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@11:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@4

check@run0:
   SUBROUTINE

   LDA   v1
   STA   HARGS
   LDA   v1+1
   STA   HARGS+1
   LDA   v1+2
   STA   HARGS+2
   LDA   v1+3
   STA   HARGS+3
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DC.L  $93310674

v1:
   DC.L  $FFFFFFFF

v2:
   DS.B  4

v3:
   DC.B  $07
   DS.B  6

v4:
   DS.B  6

v5:
   DC.L  $01077C2B
