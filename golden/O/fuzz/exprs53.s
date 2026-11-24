SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v1_0	EQU	$84
__zpabi_check__v1_1	EQU	$85
__zpabi_check__v2_0	EQU	$86
__zpabi_check__v2_1	EQU	$87
__zpabi_check__v2_2	EQU	$88
__zpabi_check__v2_3	EQU	$89
__zpabi_check__v3_0	EQU	$8A
__zpabi_check__v3_1	EQU	$8B
__zpabi_check__v4	EQU	$8C
__zpabi_check__v5_0	EQU	$8D
__zpabi_check__v5_1	EQU	$8E
__zpabi_check__v5_2	EQU	$8F
__zpabi_check__v5_3	EQU	$90
__local_check__fail_1	EQU	$92
__local_check__fail_0	EQU	$93
__local_check__0	EQU	$94
__local_check__1	EQU	$95
__local_check__2	EQU	$96
__local_check__3	EQU	$97
__local_check__4	EQU	$98
__local_check__5	EQU	$99
__local_check__6	EQU	$9A
__local_check__7	EQU	$9B

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v1_1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v1_0
   CMP   #$FF
   BNE   .lb_skip@2
   JMP   .check@asm_ssa_split@0
.lb_skip@2:
.jcmp_skip@0:
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BEQ   .lb_skip@1
   JMP   .check@asm_ssa_split@1
.lb_skip@1:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   ORA   __zpabi_check__v5_2
   ORA   __zpabi_check__v5_3
   BEQ   .lb_skip@0
   JMP   .check@asm_ssa_split@1
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
   STA   __local_check__2
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   __local_check__2
.or_end@2:
   LDA   __zpabi_check__v1_1
   BNE   .cmp_true@1
   LDA   __zpabi_check__v1_0
   BNE   .cmp_true@1
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   STA   __local_check__0
   LDA   __local_check__2
   EOR   __local_check__0
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __zpabi_check__v5_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   LDA   __local_check__1
   SBC   __zpabi_check__v5_2
   LDA   __local_check__0
   SBC   __zpabi_check__v5_3
   BCS   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   __zpabi_check__v4
   ORA   __zpabi_check__v2_0
   STA   __local_check__1
   LDA   __zpabi_check__v2_1
   STA   __local_check__0
   LDA   #$FF
   EOR   __local_check__1
   STA   __local_check__1
   LDA   #$00
   EOR   __local_check__0
   BNE   .jcmp_skip@6
   LDA   __local_check__1
   CMP   #$7F
   BEQ   .if_end@4
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v0_0
   SEC
   SBC   __zpabi_check__v2_0
   LDA   __zpabi_check__v0_1
   SBC   __zpabi_check__v2_1
   LDA   __zpabi_check__v0_2
   SBC   __zpabi_check__v2_2
   LDA   __zpabi_check__v0_3
   SBC   __zpabi_check__v2_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@7
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   SEC
   LDA   #$00
   SBC   #$01
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   AND   #$02
   STA   __local_check__7
   LDY   #$00
   LDA   __zpabi_check__v3_0
   CLC
   SBC   __zpabi_check__v2_0
   LDA   __zpabi_check__v3_1
   SBC   __zpabi_check__v2_1
   TYA
   SBC   __zpabi_check__v2_2
   TYA
   SBC   __zpabi_check__v2_3
   TYA
   ROL
   STA   __local_check__0
   LDA   #$01
   AND   __local_check__0
   STA   __local_check__3
   TYA
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__7
   AND   __local_check__3
   STA   __local_check__3
   TYA
   AND   __local_check__1
   STA   __local_check__1
   TYA
   AND   __local_check__0
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v1_0
   SEC
   SBC   #$FF
   LDA   __zpabi_check__v1_1
   SBC   #$7F
   TYA
   SBC   #$00
   TYA
   SBC   #$00
   TYA
   ROL
   STA   __local_check__3
   TYA
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_check__6
   STA   __local_check__5
   LDA   __zpabi_check__v0_0
   CLC
   ADC   __zpabi_check__v2_0
   STA   __local_check__0
   LDA   __zpabi_check__v0_1
   ADC   __zpabi_check__v2_1
   STA   __local_check__2
   LDA   __zpabi_check__v0_2
   ADC   __zpabi_check__v2_2
   STA   __local_check__1
   LDA   __zpabi_check__v0_3
   ADC   __zpabi_check__v2_3
   STA   __local_check__4
   LDA   __local_check__0
   SEC
   SBC   __zpabi_check__v4
   STA   __local_check__0
   LDA   __local_check__2
   SBC   #$00
   STA   __local_check__2
   LDA   __local_check__1
   SBC   #$00
   STA   __local_check__1
   BCS   .carry_inc@0
   DEC   __local_check__4
.carry_inc@0:
   LDA   __local_check__3
   SEC
   SBC   __local_check__0
   STA   __local_check__3
   TYA
   SBC   __local_check__2
   STA   __local_check__2
   LDA   __local_check__6
   SBC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__5
   SBC   __local_check__4
   BNE   .jcmp_skip@14
   LDA   __local_check__1
   BNE   .jcmp_skip@14
   LDA   __local_check__2
   BNE   .jcmp_skip@14
   LDA   __local_check__3
   CMP   #$73
   BEQ   .if_end@9
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v4
   STA   HARGS
   STA   HARGS+1
   JSR   udivmod8
   LDA   HARGS+2
   BEQ   .and_false@10
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   BEQ   .and_false@10
   LDA   #$01
   STA   __local_check__4
   LDA   #$00
   STA   __local_check__3
   JMP   .and_end@11
.and_false@10:
   LDA   #$00
   STA   __local_check__4
   LDY   #$00
   STY   __local_check__3
.and_end@11:
   CMP   __zpabi_check__v1_1
   BNE   .cmp_true@15
   LDA   __zpabi_check__v4
   CMP   __zpabi_check__v1_0
   BNE   .cmp_true@15
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__2
   SEC
   SBC   __zpabi_check__v5_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   LDA   __local_check__1
   SBC   __zpabi_check__v5_2
   LDA   __local_check__0
   SBC   __zpabi_check__v5_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __local_check__4
   SEC
   SBC   __local_check__0
   BCS   .carry_inc@1
   DEC   __local_check__3
.carry_inc@1:
   ORA   __local_check__3
   BEQ   .if_end@12
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   STA   HARGS+2
   LDA   __zpabi_check__v5_3
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   ORA   HARGS+13
   ORA   HARGS+14
   ORA   HARGS+15
   BEQ   .if_end@15
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BNE   .if_end@15
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@15:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .or_true@1
.check@asm_ssa_split@1:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
   JMP   .or_true@1

main:
   SUBROUTINE

   LDA   #$0E
   STA   __zpabi_check__v0_0
   LDA   #$FF
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   LDA   #$60
   STA   __zpabi_check__v3_0
   LDA   #$BA
   STA   __zpabi_check__v3_1
   LDA   #$80
   STA   __zpabi_check__v4
   LDA   #$00
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   JMP   check
