SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v2	EQU	$26
__local_check__fail	EQU	$81
__local_check__r_3	EQU	$82
__local_check__r_2	EQU	$83
__local_check__r_1	EQU	$84
__local_check__r_0	EQU	$85
__local_check__0	EQU	$86
__local_check__1	EQU	$87
__local_check__2	EQU	$88
__local_check__3	EQU	$89

; @zp-link-meta-begin
; def check params= locals=__local_check__fail,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v2 size=1 at=auto def=true init=$FF weight=3
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v4+3
   BNE   .jcmp_skip@0
   LDA   v4+2
   BNE   .jcmp_skip@0
   LDA   v4+1
   BNE   .jcmp_skip@0
   LDA   v4
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail
   LDY   #$00
.if_end@0:
   LDA   v5+3
   BNE   .jcmp_skip@1
   LDA   v5+2
   BNE   .jcmp_skip@1
   LDA   v5+1
   BNE   .jcmp_skip@1
   LDA   v5
   CMP   #$0A
   BEQ   .if_end@2
.jcmp_skip@1:
   LDA   __local_check__fail
   ORA   #$04
   STA   __local_check__fail
.if_end@2:
   LDA   v3+3
   BMI   .sx_neg@2
   TYA
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__r_1
   LDA   v3
   SEC
   SBC   v0
   LDA   v3+1
   SBC   v0+1
   LDA   v3+2
   SBC   #$00
   LDA   v3+3
   SBC   #$00
   LDA   __local_check__r_1
   SBC   #$00
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   TYA
   ROL
   STA   __local_check__r_0
   TYA
   BPL   .vthread@0
   LDA   #$FF
   BNE   .jcmp_skip@7
   TYA
   BNE   .jcmp_skip@7
.vthread@0:
   LDA   __local_check__r_0
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@7:
   LDA   __local_check__fail
   ORA   #$08
   STA   __local_check__fail
.if_end@3:
   TYA
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v5
   SEC
   SBC   v2
   LDA   v5+1
   SBC   #$00
   LDA   v5+2
   SBC   __local_check__r_2
   LDA   v5+3
   SBC   __local_check__r_3
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   TYA
   ROL
   STA   __local_check__r_3
   LDA   v0
   CLC
   SBC   __local_check__r_3
   LDA   v0+1
   SBC   #$00
   TYA
   ROL
   STA   __local_check__r_0
   LDA   v5
   ORA   v5+1
   ORA   v5+2
   ORA   v5+3
   BEQ   .and_false@4
   LDA   #$01
   STA   __local_check__r_1
   LDA   #$00
   STA   __local_check__r_2
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   STA   __local_check__r_1
   STA   __local_check__r_2
.and_end@5:
   LDA   #$7F
   SEC
   SBC   v4
   TYA
   SBC   v4+1
   TYA
   SBC   v4+2
   TYA
   SBC   v4+3
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   TYA
   ROL
   CLC
   ADC   __local_check__r_1
   STA   __local_check__r_3
   BCC   .carry_inc@0
   INC   __local_check__r_2
.carry_inc@0:
   LDA   __local_check__r_0
   SEC
   SBC   __local_check__r_3
   TYA
   SBC   __local_check__r_2
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   TYA
   ROL
   EOR   #$01
   STA   __local_check__r_3
   TYA
   BNE   .jcmp_skip@13
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@13:
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@6:
   TYA
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   __local_check__1
   STA   __local_check__0
   LDA   v5
   CLC
   ADC   v3
   STA   __local_check__r_0
   LDA   v5+1
   ADC   v3+1
   STA   __local_check__r_1
   LDA   v5+2
   ADC   v3+2
   STA   __local_check__r_2
   LDA   v5+3
   ADC   v3+3
   STA   __local_check__r_3
   LDA   v2
   SEC
   SBC   __local_check__r_0
   STA   __local_check__3
   TYA
   SBC   __local_check__r_1
   STA   __local_check__2
   LDA   __local_check__1
   SBC   __local_check__r_2
   STA   __local_check__1
   LDA   __local_check__0
   SBC   __local_check__r_3
   STA   __local_check__0
   TYA
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v4
   AND   v2
   STA   __local_check__r_0
   LDA   v4+2
   AND   __local_check__r_2
   STA   __local_check__r_2
   LDA   v4+3
   AND   __local_check__r_3
   STA   __local_check__r_3
   LDA   __local_check__3
   CLC
   SBC   __local_check__r_0
   LDA   __local_check__2
   SBC   #$00
   LDA   __local_check__1
   SBC   __local_check__r_2
   LDA   __local_check__0
   SBC   __local_check__r_3
   TYA
   ROL
   STA   __local_check__r_3
   TYA
   BNE   .jcmp_skip@18
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@18:
   LDA   __local_check__fail
   ORA   #$20
   STA   __local_check__fail
.if_end@7:
   LDA   v3+3
   BNE   .jcmp_skip@19
   LDA   v3+2
   BNE   .jcmp_skip@19
   LDA   v3+1
   BNE   .jcmp_skip@19
   LDA   v3
   CMP   #$03
   BEQ   .if_end@8
.jcmp_skip@19:
   LDA   __local_check__fail
   ORA   #$40
   STA   __local_check__fail
.if_end@8:
   LDA   v1
   AND   v4
   TAX
   LDA   #$00
   STA   __local_check__r_1
   STA   __local_check__r_0
   CPX   #$00
   BEQ   .shift_done@21
.shift_loop@20:
   ASL   __local_check__r_1
   ROL   __local_check__r_0
   DEX
   BNE   .shift_loop@20
.shift_done@21:
   LDA   __local_check__r_0
   BMI   .sx_neg@22
   TYA
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   __local_check__r_1
   ORA   __local_check__r_0
   ORA   __local_check__r_2
   ORA   __local_check__r_3
   BEQ   .if_end@9
   LDA   __local_check__fail
   ORA   #$80
   STA   __local_check__fail
.if_end@9:
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

   JMP   check

v0:
   DC.W  $0008

v1:
   DC.L  $FFFFFFFF

v3:
   DC.L  $00000003

v4:
   DC.L  $00000001

v5:
   DC.L  $0000000A

__zp_init:
   SUBROUTINE

   LDA   #$FF
   STA   v2
   RTS
