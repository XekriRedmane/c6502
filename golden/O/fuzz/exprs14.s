SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v1_2	EQU	$82
__zpabi_check__v2_0	EQU	$83
__zpabi_check__v2_1	EQU	$84
__zpabi_check__v2_2	EQU	$85
__zpabi_check__v2_3	EQU	$86
__zpabi_check__v3_0	EQU	$87
__zpabi_check__v3_1	EQU	$88
__zpabi_check__v3_2	EQU	$89
__zpabi_check__v3_3	EQU	$8A
__zpabi_check__v4	EQU	$8B
__zpabi_check__v5_0	EQU	$8C
__zpabi_check__v5_1	EQU	$8D
__zpabi_check__v5_2	EQU	$8E
__zpabi_check__v5_3	EQU	$8F
__local_check__fail_1	EQU	$91
__local_check__fail_0	EQU	$92
__local_check__0	EQU	$93
__local_check__1	EQU	$94
__local_check__2	EQU	$95
__local_check__3	EQU	$96
__local_check__4	EQU	$97
__local_check__5	EQU	$98
__local_check__6	EQU	$99
__local_check__v0	EQU	$9A

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__v0,__local_check__7,__local_check__8,__local_check__9,__local_check__10 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   LDA   __zpabi_check__v4
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__0
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   __zpabi_check__v5_0
   CLC
   ADC   __zpabi_check__v4
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   ADC   __local_check__0
   STA   __local_check__0
   LDA   __zpabi_check__v5_2
   ADC   __local_check__fail_0
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5_3
   ADC   __local_check__fail_1
   STA   __local_check__fail_1
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __local_check__1
   LDA   __zpabi_check__v3_1
   SBC   __local_check__0
   LDA   __zpabi_check__v3_2
   SBC   __local_check__fail_0
   LDA   __zpabi_check__v3_3
   SBC   __local_check__fail_1
   BCS   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   STA   HARGS+2
   LDA   __zpabi_check__v5_3
   STA   HARGS+3
   LDA   __zpabi_check__v1_0
   STA   HARGS+4
   LDA   __zpabi_check__v1_1
   STA   HARGS+5
   LDA   __zpabi_check__v1_2
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   STA   __local_check__6
   LDA   HARGS+9
   STA   __local_check__5
   LDA   HARGS+10
   STA   __local_check__4
   LDA   HARGS+11
   STA   __local_check__3
   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   __local_check__v0
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+7
   AND   #$3F
   STA   __local_check__0
   LDA   HARGS+5
   EOR   #$FF
   STA   __local_check__2
   LDA   HARGS+6
   EOR   #$FF
   STA   __local_check__1
   LDA   __local_check__0
   EOR   #$FF
   LDX   __local_check__2
   STX   HARGS+4
   LDX   __local_check__1
   STX   HARGS+5
   AND   #$3F
   STA   HARGS+6
   LDA   __local_check__6
   STA   HARGS
   LDA   __local_check__5
   STA   HARGS+1
   LDA   __local_check__4
   STA   HARGS+2
   LDA   __local_check__3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+11
   BNE   .jcmp_skip@2
   LDA   HARGS+10
   BNE   .jcmp_skip@2
   LDA   HARGS+9
   CMP   #$04
   BNE   .jcmp_skip@2
   LDA   HARGS+8
   BEQ   .if_end@1
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v3_0
   STA   HARGS
   LDA   __zpabi_check__v3_1
   STA   HARGS+1
   LDA   __zpabi_check__v3_2
   STA   HARGS+2
   LDA   __zpabi_check__v3_3
   STA   HARGS+3
   LDA   __local_check__v0
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   EOR   #$FF
   STA   __local_check__6
   LDA   HARGS+6
   EOR   #$FF
   STA   __local_check__5
   LDA   HARGS+7
   EOR   #$FF
   STA   __local_check__4
   LDA   HARGS+8
   EOR   #$FF
   STA   __local_check__3
   LDA   #$00
   SEC
   SBC   __zpabi_check__v3_0
   LDA   #$00
   SBC   __zpabi_check__v3_1
   LDA   #$00
   SBC   __zpabi_check__v3_2
   LDA   #$80
   SBC   __zpabi_check__v3_3
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   __zpabi_check__v4
   LDX   __zpabi_check__v3_0
   BEQ   .shift_done@4
.shift_loop@3:
   ASL
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   AND   #$1F
   EOR   #$10
   SEC
   SBC   #$10
   STA   __local_check__1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__0
   LDA   __local_check__1
   SEC
   SBC   __local_check__2
   LDA   __local_check__0
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   STA   HARGS+4
   LDA   __local_check__6
   STA   HARGS
   LDA   __local_check__5
   STA   HARGS+1
   LDA   __local_check__4
   STA   HARGS+2
   LDA   __local_check__3
   STA   HARGS+3
   JSR   lsr32
   LDA   HARGS+8
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+7
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+6
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+5
   CMP   #$FF
   BEQ   .vthread@0
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.vthread@0:
   LDA   #$FF
   BMI   .sx_neg@11
   LDA   #$00
.sx_neg@11:
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$FF
   EOR   #$FF
   STA   __local_check__3
   LDA   #$FF
   EOR   #$7F
   STA   __local_check__2
   LDA   __local_check__1
   EOR   #$3F
   STA   __local_check__1
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __local_check__3
   LDA   __zpabi_check__v5_1
   SBC   __local_check__2
   LDA   __zpabi_check__v5_2
   SBC   __local_check__1
   LDA   __zpabi_check__v5_3
   SBC   __local_check__0
   BCC   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v5_0
   STA   __local_check__5
   LDA   __zpabi_check__v5_1
   STA   __local_check__4
   LDA   __zpabi_check__v5_2
   AND   #$3F
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   LDX   __local_check__v0
   STX   __local_check__2
   AND   #$3F
   STA   __local_check__0
   LDA   __local_check__5
   SEC
   SBC   __local_check__2
   STA   __local_check__2
   LDA   __local_check__4
   SBC   #$00
   STA   __local_check__1
   LDA   __local_check__3
   SBC   __local_check__0
   LDX   __local_check__2
   STX   __local_check__4
   LDX   __local_check__1
   STX   __local_check__3
   AND   #$3F
   STA   __local_check__2
   LDA   __zpabi_check__v4
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   HARGS+5
   LDX   __zpabi_check__v4
   STX   HARGS+4
   AND   #$3F
   STA   HARGS+6
   LDA   __local_check__4
   STA   HARGS
   LDA   __local_check__3
   STA   HARGS+1
   LDA   __local_check__2
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   ORA   HARGS+13
   ORA   HARGS+14
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

main:
   SUBROUTINE

   LDA   #$20
   STA   __zpabi_check__v1_2
   LDA   #$46
   STA   __zpabi_check__v2_0
   LDA   #$06
   STA   __zpabi_check__v2_1
   LDA   #$80
   STA   __zpabi_check__v2_2
   LDA   #$4A
   STA   __zpabi_check__v2_3
   LDA   #$0C
   STA   __zpabi_check__v4
   LDA   #$00
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   LDY   #$80
   STY   __zpabi_check__v5_3
   JMP   check
