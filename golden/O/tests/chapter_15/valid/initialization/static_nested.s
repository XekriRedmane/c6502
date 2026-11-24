SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_test_global__0	EQU	$82
__local_test_local__0	EQU	$82
__local_test_global__1	EQU	$83
__local_test_local__1	EQU	$83
__local_check_long_arr__0	EQU	$84
__local_check_ulong_arr__0	EQU	$84
__local_check_double_arr__1	EQU	$85
__local_check_long_arr__1	EQU	$85
__local_check_ulong_arr__1	EQU	$85
__local_check_double_arr__2	EQU	$86
__local_check_long_arr__2	EQU	$86
__local_check_ulong_arr__2	EQU	$86
__local_check_double_arr__3	EQU	$87
__local_check_long_arr__3	EQU	$87
__local_check_ulong_arr__3	EQU	$87
__local_check_double_arr__4	EQU	$88
__local_check_long_arr__4	EQU	$88
__local_check_ulong_arr__4	EQU	$88
__local_check_double_arr__5	EQU	$89
__local_check_long_arr__5	EQU	$89
__local_check_ulong_arr__5	EQU	$89
__local_check_double_arr__6	EQU	$8A
__local_check_long_arr__k_1	EQU	$8A
__local_check_ulong_arr__k_1	EQU	$8A
__local_check_double_arr__7	EQU	$8B
__local_check_long_arr__k_0	EQU	$8B
__local_check_ulong_arr__k_0	EQU	$8B
__local_check_double_arr__arr	EQU	$8C
__local_check_long_arr__j_1	EQU	$8C
__local_check_ulong_arr__j_1	EQU	$8C
__local_check_double_arr__8	EQU	$8D
__local_check_long_arr__j_0	EQU	$8D
__local_check_ulong_arr__j_0	EQU	$8D
__local_check_long_arr__i_1	EQU	$8E
__local_check_ulong_arr__i_1	EQU	$8E
__local_check_long_arr__i_0	EQU	$8F
__local_check_ulong_arr__i_0	EQU	$8F
__local_check_long_arr__arr_1	EQU	$90
__local_check_long_arr__arr_0	EQU	$91
__local_check_ulong_arr__arr_0	EQU	$91

; @zp-link-meta-begin
; def check_double_arr params=__zpabi_check_double_arr__arr_0,__zpabi_check_double_arr__arr_1 locals=__local_check_double_arr__0,__local_check_double_arr__1,__local_check_double_arr__2,__local_check_double_arr__3,__local_check_double_arr__4,__local_check_double_arr__5,__local_check_double_arr__6,__local_check_double_arr__7,__local_check_double_arr__arr,__local_check_double_arr__8 indirect=false in_cycle=false param_regs=A,X
; def check_long_arr params=__zpabi_check_long_arr__arr_0,__zpabi_check_long_arr__arr_1 locals=__local_check_long_arr__0,__local_check_long_arr__1,__local_check_long_arr__2,__local_check_long_arr__3,__local_check_long_arr__4,__local_check_long_arr__5,__local_check_long_arr__k_1,__local_check_long_arr__k_0,__local_check_long_arr__j_1,__local_check_long_arr__j_0,__local_check_long_arr__i_1,__local_check_long_arr__i_0,__local_check_long_arr__arr_1,__local_check_long_arr__arr_0,__local_check_long_arr__6,__local_check_long_arr__7 indirect=false in_cycle=false param_regs=A,X
; def check_ulong_arr params=__zpabi_check_ulong_arr__arr_0,__zpabi_check_ulong_arr__arr_1 locals=__local_check_ulong_arr__0,__local_check_ulong_arr__1,__local_check_ulong_arr__2,__local_check_ulong_arr__3,__local_check_ulong_arr__4,__local_check_ulong_arr__5,__local_check_ulong_arr__k_1,__local_check_ulong_arr__k_0,__local_check_ulong_arr__j_1,__local_check_ulong_arr__j_0,__local_check_ulong_arr__i_1,__local_check_ulong_arr__i_0,__local_check_ulong_arr__arr_1,__local_check_ulong_arr__arr_0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_global params= locals=__local_test_global__0,__local_test_global__1 indirect=false in_cycle=false
; def test_local params= locals=__local_test_local__0,__local_test_local__1 indirect=false in_cycle=false
; call main -> test_global
; call main -> test_local
; call test_global -> check_double_arr
; call test_global -> check_long_arr
; call test_global -> check_ulong_arr
; call test_local -> check_double_arr
; call test_local -> check_long_arr
; call test_local -> check_ulong_arr
; item check_double_arr section=.text local=false zero=false
; item check_double_arr@tail0 section=.text local=true zero=false
; item check_long_arr section=.text local=false zero=false
; item check_ulong_arr section=.text local=false zero=false
; item test_global section=.text local=false zero=false
; item test_local section=.text local=false zero=false
; item main section=.text local=false zero=false
; item double_arr section=.data local=false zero=false
; item long_arr section=.bss local=false zero=true
; item ulong_arr section=.data local=false zero=false
; item @11.local_double_arr section=.data local=true zero=false
; item @13.local_long_arr section=.bss local=true zero=true
; item @14.local_ulong_arr section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

check_double_arr:
   SUBROUTINE

   STA   __local_check_double_arr__arr
   STX   __local_check_double_arr__8
   LDY   #$00
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__7
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__6
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__5
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__4
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__3
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__2
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__1
   INY
   LDA   (__local_check_double_arr__arr),Y
   CMP   #$3F
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__1
   CMP   #$F1
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__2
   CMP   #$99
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__3
   CMP   #$99
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__4
   CMP   #$99
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__5
   CMP   #$99
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__6
   CMP   #$99
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__7
   CMP   #$9A
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$08
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__7
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__6
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__5
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__4
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__3
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__2
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__1
   INY
   LDA   (__local_check_double_arr__arr),Y
   CMP   #$40
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__1
   CMP   #$01
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__2
   CMP   #$99
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__3
   CMP   #$99
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__4
   CMP   #$99
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__5
   CMP   #$99
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__6
   CMP   #$99
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__7
   CMP   #$9A
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDY   #$10
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__7
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__6
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__5
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__4
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__3
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__2
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__1
   INY
   LDA   (__local_check_double_arr__arr),Y
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__1
   CMP   #$0A
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__2
   CMP   #$66
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__3
   CMP   #$66
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__4
   CMP   #$66
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__5
   CMP   #$66
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__6
   CMP   #$66
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__7
   CMP   #$66
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDY   #$18
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__7
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__6
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__5
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__4
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__3
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__2
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__1
   INY
   LDA   (__local_check_double_arr__arr),Y
   CMP   #$40
   BNE   .jcmp_skip@3
   LDA   __local_check_double_arr__1
   CMP   #$11
   BNE   .jcmp_skip@3
   LDA   __local_check_double_arr__2
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   __local_check_double_arr__3
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   __local_check_double_arr__4
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   __local_check_double_arr__5
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   __local_check_double_arr__6
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   __local_check_double_arr__7
   CMP   #$9A
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JMP   check_double_arr@tail0

check_double_arr@tail0:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

check_long_arr:
   SUBROUTINE

   STA   __local_check_long_arr__arr_0
   STX   __local_check_long_arr__arr_1
   LDA   #$00
   STA   __local_check_long_arr__i_0
   LDY   #$00
   STY   __local_check_long_arr__i_1
.loop@0_start:
   LDA   __local_check_long_arr__i_0
   SEC
   SBC   #$1E
   LDA   __local_check_long_arr__i_1
   SBC   #$00
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BMI   .lb_skip@5
   JMP   .loop@0_break
.lb_skip@5:
   LDA   #$00
   STA   __local_check_long_arr__j_0
   LDY   #$00
   STY   __local_check_long_arr__j_1
.loop@1_start:
   LDA   __local_check_long_arr__j_0
   SEC
   SBC   #$32
   LDA   __local_check_long_arr__j_1
   SBC   #$00
   BVC   .jcmp_novf@5
   EOR   #$80
.jcmp_novf@5:
   BMI   .lb_skip@4
   JMP   .loop@1_break
.lb_skip@4:
   LDA   #$00
   STA   __local_check_long_arr__k_0
   LDY   #$00
   STY   __local_check_long_arr__k_1
.loop@2_start:
   LDA   __local_check_long_arr__k_0
   SEC
   SBC   #$28
   LDA   __local_check_long_arr__k_1
   SBC   #$00
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BMI   .lb_skip@3
   JMP   .loop@2_break
.lb_skip@3:
   LDA   __local_check_long_arr__i_0
   STA   HARGS
   LDA   __local_check_long_arr__i_1
   STA   HARGS+1
   LDA   #$40
   STA   HARGS+2
   LDA   #$1F
   STA   HARGS+3
   JSR   mul16
   LDA   __local_check_long_arr__arr_0
   CLC
   ADC   HARGS+4
   STA   __local_check_long_arr__3
   LDA   __local_check_long_arr__arr_1
   ADC   HARGS+5
   STA   __local_check_long_arr__2
   LDA   __local_check_long_arr__j_0
   STA   HARGS
   LDA   __local_check_long_arr__j_1
   STA   HARGS+1
   LDA   #$A0
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   __local_check_long_arr__3
   CLC
   ADC   HARGS+4
   STA   __local_check_long_arr__1
   LDA   __local_check_long_arr__2
   ADC   HARGS+5
   STA   __local_check_long_arr__5
   LDA   __local_check_long_arr__k_0
   ASL
   ASL
   CLC
   ADC   __local_check_long_arr__1
   STA   __local_check_long_arr__4
   BCC   .carry_inc@0
   INC   __local_check_long_arr__5
.carry_inc@0:
   LDY   #$01
   LDA   (__local_check_long_arr__4),Y
   STA   __local_check_long_arr__2
   INY
   LDA   (__local_check_long_arr__4),Y
   STA   __local_check_long_arr__1
   INY
   LDA   (__local_check_long_arr__4),Y
   STA   __local_check_long_arr__0
   LDY   #$00
   LDA   (__local_check_long_arr__4),Y
   ORA   __local_check_long_arr__2
   ORA   __local_check_long_arr__1
   ORA   __local_check_long_arr__0
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   INC   __local_check_long_arr__k_0
   BEQ   .lb_skip@2
   JMP   .loop@2_start
.lb_skip@2:
   INC   __local_check_long_arr__k_1
   JMP   .loop@2_start
.loop@2_break:
   INC   __local_check_long_arr__j_0
   BEQ   .lb_skip@1
   JMP   .loop@1_start
.lb_skip@1:
   INC   __local_check_long_arr__j_1
   JMP   .loop@1_start
.loop@1_break:
   INC   __local_check_long_arr__i_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_check_long_arr__i_1
   JMP   .loop@0_start
.loop@0_break:
   JMP   check_double_arr@tail0

check_ulong_arr:
   SUBROUTINE

   STA   __local_check_ulong_arr__arr_0
   LDA   #$00
   STA   __local_check_ulong_arr__i_0
   LDY   #$00
   STY   __local_check_ulong_arr__i_1
.loop@3_start:
   LDA   __local_check_ulong_arr__i_0
   SEC
   SBC   #$04
   LDA   __local_check_ulong_arr__i_1
   SBC   #$00
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BMI   .lb_skip@11
   JMP   .loop@3_break
.lb_skip@11:
   LDA   #$00
   STA   __local_check_ulong_arr__j_0
   LDY   #$00
   STY   __local_check_ulong_arr__j_1
.loop@4_start:
   LDA   __local_check_ulong_arr__j_0
   SEC
   SBC   #$06
   LDA   __local_check_ulong_arr__j_1
   SBC   #$00
   BVC   .jcmp_novf@8
   EOR   #$80
.jcmp_novf@8:
   BMI   .lb_skip@10
   JMP   .loop@4_break
.lb_skip@10:
   LDA   #$00
   STA   __local_check_ulong_arr__k_0
   LDY   #$00
   STY   __local_check_ulong_arr__k_1
.loop@5_start:
   LDA   __local_check_ulong_arr__k_0
   SEC
   SBC   #$02
   LDA   __local_check_ulong_arr__k_1
   SBC   #$00
   BVC   .jcmp_novf@9
   EOR   #$80
.jcmp_novf@9:
   BMI   .lb_skip@9
   JMP   .loop@5_break
.lb_skip@9:
   LDA   __local_check_ulong_arr__i_0
   ASL
   STA   __local_check_ulong_arr__4
   LDA   __local_check_ulong_arr__i_1
   ROL
   ASL   __local_check_ulong_arr__4
   ROL
   ASL   __local_check_ulong_arr__4
   ROL
   ASL   __local_check_ulong_arr__4
   ROL
   STA   __local_check_ulong_arr__5
   LDA   __local_check_ulong_arr__4
   ASL
   STA   __local_check_ulong_arr__2
   LDA   __local_check_ulong_arr__5
   ROL
   STA   __local_check_ulong_arr__3
   LDA   __local_check_ulong_arr__2
   CLC
   ADC   __local_check_ulong_arr__4
   STA   __local_check_ulong_arr__1
   LDA   __local_check_ulong_arr__3
   ADC   __local_check_ulong_arr__5
   STA   __local_check_ulong_arr__0
   LDA   __local_check_ulong_arr__arr_0
   CLC
   ADC   __local_check_ulong_arr__1
   STA   __local_check_ulong_arr__3
   TXA
   ADC   __local_check_ulong_arr__0
   STA   __local_check_ulong_arr__1
   LDA   __local_check_ulong_arr__j_0
   ASL
   ASL
   ASL
   CLC
   ADC   __local_check_ulong_arr__3
   STA   __local_check_ulong_arr__3
   BCC   .carry_inc@1
   INC   __local_check_ulong_arr__1
.carry_inc@1:
   LDA   __local_check_ulong_arr__k_0
   ASL
   ASL
   CLC
   ADC   __local_check_ulong_arr__3
   STA   __local_check_ulong_arr__0
   BCC   .carry_inc@2
   INC   __local_check_ulong_arr__1
.carry_inc@2:
   LDY   #$00
   LDA   (__local_check_ulong_arr__0),Y
   STA   __local_check_ulong_arr__3
   INY
   LDA   (__local_check_ulong_arr__0),Y
   STA   __local_check_ulong_arr__2
   LDA   __local_check_ulong_arr__i_0
   ORA   __local_check_ulong_arr__i_1
   BNE   .if_else@17
   LDA   __local_check_ulong_arr__j_0
   ORA   __local_check_ulong_arr__j_1
   BNE   .check_ulong_arr@cthread@0
   LDA   __local_check_ulong_arr__k_0
   ORA   __local_check_ulong_arr__k_1
   BNE   .if_else@24
   LDA   __local_check_ulong_arr__2
   BMI   .jcmp_skip@12
   CMP   #$03
   BNE   .jcmp_skip@12
   LDA   __local_check_ulong_arr__3
   CMP   #$E8
   BEQ   .if_end@9
.jcmp_skip@12:
   LDX   #$00
   LDA   #$06
   RTS
.check_ulong_arr@cthread@0:
   LDA   __local_check_ulong_arr__j_1
   BNE   .if_else@24
   LDA   __local_check_ulong_arr__j_0
   CMP   #$01
   BNE   .if_else@24
   LDA   __local_check_ulong_arr__k_0
   ORA   __local_check_ulong_arr__k_1
   BNE   .if_else@24
   LDA   __local_check_ulong_arr__2
   BMI   .jcmp_skip@15
   BNE   .jcmp_skip@15
   LDA   __local_check_ulong_arr__3
   CMP   #$0C
   BEQ   .if_end@16
.jcmp_skip@15:
   LDX   #$00
   LDA   #$07
   RTS
.if_else@17:
   LDA   __local_check_ulong_arr__i_1
   BNE   .if_else@24
   LDA   __local_check_ulong_arr__i_0
   CMP   #$01
   BNE   .if_else@24
   LDA   __local_check_ulong_arr__j_0
   ORA   __local_check_ulong_arr__j_1
   BNE   .if_else@24
   LDA   __local_check_ulong_arr__k_0
   ORA   __local_check_ulong_arr__k_1
   BNE   .if_else@24
   LDA   __local_check_ulong_arr__2
   BMI   .jcmp_skip@18
   BNE   .jcmp_skip@18
   LDA   __local_check_ulong_arr__3
   CMP   #$02
   BEQ   .if_end@23
.jcmp_skip@18:
   LDX   #$00
   LDA   #$08
   RTS
.if_else@24:
   LDA   __local_check_ulong_arr__3
   ORA   __local_check_ulong_arr__2
   BEQ   .if_end@26
   LDX   #$00
   LDA   #$09
   RTS
.if_end@26:
.if_end@23:
.if_end@16:
.if_end@9:
   INC   __local_check_ulong_arr__k_0
   BEQ   .lb_skip@8
   JMP   .loop@5_start
.lb_skip@8:
   INC   __local_check_ulong_arr__k_1
   JMP   .loop@5_start
.loop@5_break:
   INC   __local_check_ulong_arr__j_0
   BEQ   .lb_skip@7
   JMP   .loop@4_start
.lb_skip@7:
   INC   __local_check_ulong_arr__j_1
   JMP   .loop@4_start
.loop@4_break:
   INC   __local_check_ulong_arr__i_0
   BEQ   .lb_skip@6
   JMP   .loop@3_start
.lb_skip@6:
   INC   __local_check_ulong_arr__i_1
   JMP   .loop@3_start
.loop@3_break:
   JMP   check_double_arr@tail0

test_global:
   SUBROUTINE

   LDX   #>double_arr
   LDA   #<double_arr
   JSR   check_double_arr
   STA   __local_test_global__1
   STX   __local_test_global__0
   ORA   __local_test_global__0
   BNE   .tail@0
   LDX   #>long_arr
   LDA   #<long_arr
   JSR   check_long_arr
   STA   __local_test_global__1
   STX   __local_test_global__0
   ORA   __local_test_global__0
   BNE   .tail@0
   LDX   #>ulong_arr
   LDA   #<ulong_arr
   JSR   check_ulong_arr
   STA   __local_test_global__1
   STX   __local_test_global__0
   ORA   __local_test_global__0
   BEQ   .if_end@29
.tail@0:
   LDA   __local_test_global__1
   RTS
.if_end@29:
   JMP   check_double_arr@tail0

test_local:
   SUBROUTINE

   LDX   #>@11.local_double_arr
   LDA   #<@11.local_double_arr
   JSR   check_double_arr
   STA   __local_test_local__0
   STX   __local_test_local__1
   ORA   __local_test_local__1
   BEQ   .if_end@30
   LDA   #$64
   CLC
   ADC   __local_test_local__0
   BCC   .carry_inc@3
   INX
.carry_inc@3:
   RTS
.if_end@30:
   LDX   #>@13.local_long_arr
   LDA   #<@13.local_long_arr
   JSR   check_long_arr
   STA   __local_test_local__0
   STX   __local_test_local__1
   ORA   __local_test_local__1
   BEQ   .if_end@31
   LDA   #$64
   CLC
   ADC   __local_test_local__0
   BCC   .carry_inc@4
   INX
.carry_inc@4:
   RTS
.if_end@31:
   LDX   #>@14.local_ulong_arr
   LDA   #<@14.local_ulong_arr
   JSR   check_ulong_arr
   STA   __local_test_local__0
   STX   __local_test_local__1
   ORA   __local_test_local__1
   BEQ   .if_end@32
   LDA   #$64
   CLC
   ADC   __local_test_local__0
   BCC   .carry_inc@5
   INX
.carry_inc@5:
   RTS
.if_end@32:
   JMP   check_double_arr@tail0

main:
   SUBROUTINE

   JSR   test_global
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@33
   LDA   __local_main__1
   RTS
.if_end@33:
   JMP   test_local

double_arr:
   DC.L  $9999999A
   DC.L  $3FF19999
   DC.L  $9999999A
   DC.L  $40019999
   DC.L  $66666666
   DC.L  $400A6666
   DC.L  $9999999A
   DC.L  $40119999

long_arr:
   DS.B  240000

ulong_arr:
   DC.L  $000003E8
   DS.B  4
   DC.L  $0000000C
   DS.B  36
   DC.L  $00000002
   DS.B  140

@11.local_double_arr:
   DC.L  $9999999A
   DC.L  $3FF19999
   DC.L  $9999999A
   DC.L  $40019999
   DC.L  $66666666
   DC.L  $400A6666
   DC.L  $9999999A
   DC.L  $40119999

@13.local_long_arr:
   DS.B  240000

@14.local_ulong_arr:
   DC.L  $000003E8
   DS.B  4
   DC.L  $0000000C
   DS.B  36
   DC.L  $00000002
   DS.B  140
