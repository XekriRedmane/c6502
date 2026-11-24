SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2	EQU	$82
__zpabi_check__v3_0	EQU	$83
__zpabi_check__v3_1	EQU	$84
__zpabi_check__v3_2	EQU	$85
__zpabi_check__v3_3	EQU	$86
__zpabi_check__v4_0	EQU	$87
__zpabi_check__v4_1	EQU	$88
__zpabi_check__v4_2	EQU	$89
__zpabi_check__v4_3	EQU	$8A
__zpabi_check__v5	EQU	$8B
__local_check__fail_1	EQU	$8D
__local_check__fail_0	EQU	$8E
__local_check__0	EQU	$8F
__local_check__1	EQU	$90
__local_check__2	EQU	$91
__local_check__3	EQU	$92
__local_check__4	EQU	$93
__local_check__5	EQU	$94
__local_check__v0	EQU	$95

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__v0 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   LDA   __zpabi_check__v3_0
   EOR   #$07
   STA   __local_check__5
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   STA   __local_check__4
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_check__3
   STA   __local_check__2
   LDA   __zpabi_check__v3_0
   STA   HARGS
   LDA   __zpabi_check__v3_1
   STA   HARGS+1
   LDA   __zpabi_check__v3_2
   STA   HARGS+2
   LDA   __zpabi_check__v3_3
   STA   HARGS+3
   LDA   #$7F
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   __local_check__4
   SEC
   SBC   HARGS+8
   LDA   #$00
   SBC   HARGS+9
   LDA   __local_check__3
   SBC   HARGS+10
   LDA   __local_check__2
   SBC   HARGS+11
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   AND   __local_check__5
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   SEC
   SBC   __zpabi_check__v5
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   BMI   .if_end@1
   ORA   __local_check__1
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   #$7F
   STA   HARGS
   LDA   #$FF
   STA   HARGS+1
   LDY   #$FF
   STY   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v3_0
   STA   HARGS+4
   LDA   __zpabi_check__v3_1
   STA   HARGS+5
   LDA   __zpabi_check__v3_2
   STA   HARGS+6
   LDA   __zpabi_check__v3_3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+11
   BNE   .jcmp_skip@5
   LDA   HARGS+10
   BNE   .jcmp_skip@5
   LDA   HARGS+9
   BNE   .jcmp_skip@5
   LDA   HARGS+8
   CMP   #$81
   BEQ   .if_end@2
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   #$00
   CMP   __zpabi_check__v4_3
   BNE   .cmp_true@6
   CMP   __zpabi_check__v4_2
   BNE   .cmp_true@6
   LDA   #$FE
   CMP   __zpabi_check__v4_1
   BNE   .cmp_true@6
   LDA   #$FF
   CMP   __zpabi_check__v4_0
   BNE   .cmp_true@6
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   HARGS+4
   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   __zpabi_check__v4_1
   STA   HARGS+1
   LDA   __zpabi_check__v4_2
   STA   HARGS+2
   LDA   __zpabi_check__v4_3
   STA   HARGS+3
   JSR   asr32
   LDA   HARGS+5
   ORA   HARGS+6
   ORA   HARGS+7
   ORA   HARGS+8
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v2
   CMP   #$80
   BEQ   .vthread@0
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
   LDA   __zpabi_check__v2
.vthread@0:
   ORA   __zpabi_check__v1_0
   STA   __local_check__3
   LDA   __zpabi_check__v1_0
   SEC
   SBC   #$01
   STA   __local_check__1
   LDA   __zpabi_check__v1_1
   SBC   #$00
   STA   __local_check__0
   SEC
   LDA   #$00
   SBC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   STA   __local_check__0
   LDA   __local_check__3
   AND   __local_check__1
   STA   __local_check__1
   LDA   __zpabi_check__v1_1
   AND   __local_check__0
   BNE   .jcmp_skip@9
   LDA   __local_check__1
   CMP   #$82
   BEQ   .if_end@5
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __local_check__v0
   EOR   __zpabi_check__v5
   CMP   #$B2
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

   LDA   #$0F
   STA   __zpabi_check__v1_0
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$80
   STA   __zpabi_check__v2
   LDA   #$FF
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   LDA   #$01
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v5
   LDA   #$B3
   JMP   check
