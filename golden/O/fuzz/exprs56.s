SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2_0	EQU	$82
__zpabi_check__v2_1	EQU	$83
__zpabi_check__v2_2	EQU	$84
__zpabi_check__v2_3	EQU	$85
__zpabi_check__v2_4	EQU	$86
__zpabi_check__v2_5	EQU	$87
__zpabi_check__v3	EQU	$88
__zpabi_check__v4_0	EQU	$89
__zpabi_check__v4_1	EQU	$8A
__zpabi_check__v5_0	EQU	$8B
__zpabi_check__v5_1	EQU	$8C
__local_check__fail	EQU	$8E
__local_check__0	EQU	$8F
__local_check__1	EQU	$90
__local_check__2	EQU	$91
__local_check__3	EQU	$92
__local_check__4	EQU	$93
__local_check__5	EQU	$94
__local_check__v0_1	EQU	$95
__local_check__v0_0	EQU	$96

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v2_4,__zpabi_check__v2_5,__zpabi_check__v3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v4_0
   STA   __local_check__0
   LDA   __zpabi_check__v1_1
   ORA   __zpabi_check__v4_1
   BNE   .jcmp_skip@0
   LDA   __local_check__0
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail
   LDY   #$00
.if_end@0:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .if_end@3
   LDA   __zpabi_check__v5_0
   CLC
   ADC   __zpabi_check__v2_0
   STA   __local_check__5
   LDA   __zpabi_check__v5_1
   ADC   __zpabi_check__v2_1
   STA   __local_check__4
   TYA
   ADC   __zpabi_check__v2_2
   STA   __local_check__3
   TYA
   ADC   __zpabi_check__v2_3
   STA   __local_check__2
   TYA
   ADC   __zpabi_check__v2_4
   STA   __local_check__1
   TYA
   ADC   __zpabi_check__v2_5
   STA   __local_check__0
   LDA   __zpabi_check__v4_0
   CLC
   ADC   __local_check__5
   STA   __local_check__5
   LDA   __zpabi_check__v4_1
   ADC   __local_check__4
   STA   __local_check__4
   TYA
   ADC   __local_check__3
   STA   __local_check__3
   TYA
   ADC   __local_check__2
   STA   __local_check__2
   TYA
   ADC   __local_check__1
   STA   __local_check__1
   TYA
   ADC   __local_check__0
   STA   __local_check__0
   LDA   __local_check__5
   ORA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@3
   LDA   __local_check__fail
   ORA   #$02
   STA   __local_check__fail
.if_end@3:
   LDA   __zpabi_check__v3
   BEQ   .lnot_true@1
   TYA
   JMP   .lnot_end@2
.lnot_true@1:
   LDA   #$01
.lnot_end@2:
   STA   __local_check__0
   TYA
   CMP   #$FF
   BNE   .cmp_end@4
   LDA   __local_check__0
   CMP   #$FF
   BEQ   .cmp_true@3
   TYA
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_check__0
   LDA   #$00
   STA   __local_check__1
   LDX   __zpabi_check__v1_0
   BEQ   .shift_done@7
.shift_loop@6:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@4
   LDA   __local_check__fail
   ORA   #$04
   STA   __local_check__fail
.if_end@4:
   LDA   __zpabi_check__v1_0
   SEC
   SBC   __zpabi_check__v5_0
   LDA   __zpabi_check__v1_1
   SBC   __zpabi_check__v5_1
   TYA
   ROL
   STA   __local_check__1
   LDA   #$0E
   SEC
   SBC   __zpabi_check__v1_0
   LDA   #$0C
   SBC   __zpabi_check__v1_1
   TYA
   ROL
   STA   __local_check__0
   LDA   __local_check__1
   EOR   __local_check__0
   STA   __local_check__1
   LDA   #$07
   SEC
   SBC   __local_check__1
   STA   __local_check__1
   TYA
   SBC   #$00
   BNE   .jcmp_skip@8
   LDA   __local_check__1
   CMP   #$07
   BEQ   .if_end@10
.jcmp_skip@8:
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@10:
   LDA   __zpabi_check__v3
   CLC
   SBC   __zpabi_check__v4_0
   TYA
   SBC   __zpabi_check__v4_1
   TYA
   ROL
   STA   __local_check__1
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .lnot_true@9
   TYA
   JMP   .lnot_end@10
.lnot_true@9:
   LDA   #$01
.lnot_end@10:
   STA   __local_check__0
   LDA   __local_check__1
   SEC
   SBC   __local_check__0
   TYA
   SBC   #$00
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   TYA
   ROL
   EOR   #$01
   STA   __local_check__4
   TYA
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__4
   EOR   __zpabi_check__v2_0
   STA   __local_check__5
   TYA
   EOR   __zpabi_check__v2_1
   STA   __local_check__4
   LDA   __local_check__3
   EOR   __zpabi_check__v2_2
   STA   __local_check__3
   LDA   __local_check__2
   EOR   __zpabi_check__v2_3
   STA   __local_check__2
   LDA   __local_check__1
   EOR   __zpabi_check__v2_4
   STA   __local_check__1
   LDA   __local_check__0
   EOR   __zpabi_check__v2_5
   CMP   #$80
   BNE   .jcmp_skip@14
   LDA   __local_check__1
   BNE   .jcmp_skip@14
   LDA   __local_check__2
   BNE   .jcmp_skip@14
   LDA   __local_check__3
   BNE   .jcmp_skip@14
   LDA   __local_check__4
   BNE   .jcmp_skip@14
   LDA   __local_check__5
   CMP   #$01
   BEQ   .if_end@11
.jcmp_skip@14:
   LDA   __local_check__fail
   ORA   #$20
   STA   __local_check__fail
.if_end@11:
   LDA   __zpabi_check__v4_0
   CLC
   SBC   __zpabi_check__v1_0
   LDA   __zpabi_check__v4_1
   SBC   __zpabi_check__v1_1
   TYA
   ROL
   STA   __local_check__0
   TYA
   SEC
   SBC   __local_check__0
   TYA
   SBC   #$00
   TYA
   ROL
   STA   __local_check__4
   TYA
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v2_0
   AND   __local_check__4
   STA   __local_check__5
   LDA   __zpabi_check__v2_2
   AND   __local_check__3
   STA   __local_check__3
   LDA   __zpabi_check__v2_3
   AND   __local_check__2
   STA   __local_check__2
   LDA   __zpabi_check__v2_4
   AND   __local_check__1
   STA   __local_check__1
   LDA   __zpabi_check__v2_5
   AND   __local_check__0
   STA   __local_check__0
   LDA   __local_check__5
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@12
   LDA   __local_check__fail
   ORA   #$40
   STA   __local_check__fail
.if_end@12:
   LDA   #$59
   CLC
   SBC   __local_check__v0_0
   LDA   #$89
   SBC   __local_check__v0_1
   LDA   #$2E
   SBC   #$00
   LDA   #$7F
   SBC   #$00
   LDA   #$5D
   SBC   #$00
   LDA   #$13
   SBC   #$00
   LDA   #$29
   SBC   #$00
   LDA   #$0B
   SBC   #$00
   TYA
   ROL
   STA   __local_check__v0_1
   TYA
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   LDA   __local_check__v0_1
   STA   __local_check__v0_0
   LDA   __local_check__5
   AND   #$0F
   STA   __local_check__0
   LDA   #$C0
   CLC
   ADC   __local_check__v0_0
   STA   __local_check__v0_0
   LDA   #$27
   ADC   #$00
   STA   __local_check__v0_1
   LDA   #$AD
   ADC   __local_check__5
   STA   __local_check__5
   LDA   #$9B
   ADC   __local_check__4
   STA   __local_check__4
   LDA   #$69
   ADC   __local_check__3
   STA   __local_check__3
   LDA   #$90
   ADC   __local_check__2
   STA   __local_check__2
   LDA   #$1B
   ADC   __local_check__1
   STA   __local_check__1
   TYA
   ADC   __local_check__0
   AND   #$0F
   BNE   .jcmp_skip@19
   LDA   __local_check__1
   CMP   #$1B
   BNE   .jcmp_skip@19
   LDA   __local_check__2
   CMP   #$90
   BNE   .jcmp_skip@19
   LDA   __local_check__3
   CMP   #$69
   BNE   .jcmp_skip@19
   LDA   __local_check__4
   CMP   #$9B
   BNE   .jcmp_skip@19
   LDA   __local_check__5
   CMP   #$AD
   BNE   .jcmp_skip@19
   LDA   __local_check__v0_1
   CMP   #$27
   BNE   .jcmp_skip@19
   LDA   __local_check__v0_0
   CMP   #$C1
   BEQ   .if_end@13
.jcmp_skip@19:
   LDA   __local_check__fail
   ORA   #$80
   STA   __local_check__fail
.if_end@13:
   TYA
   TAX
   LDA   __local_check__fail
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail
   LDY   #$00
   JMP   .if_end@0

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v5_0
   LDA   #$80
   STA   __zpabi_check__v2_5
   STA   __zpabi_check__v3
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v2_4
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v5_1
   LDX   #$00
   JMP   check
