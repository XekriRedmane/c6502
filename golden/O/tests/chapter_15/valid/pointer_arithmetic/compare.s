SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_ge__b_0	EQU	$80
__zpabi_ge__b_1	EQU	$81
__zpabi_ge_nested__b_0	EQU	$82
__zpabi_ge_nested__b_1	EQU	$83
__zpabi_gt__b_0	EQU	$84
__zpabi_gt__b_1	EQU	$85
__local_ge__a_1	EQU	$86
__local_ge_nested__a_1	EQU	$86
__local_gt__a_1	EQU	$86
__local_lt__a_1	EQU	$86
__zpabi_gt_nested__b_0	EQU	$86
__zpabi_le__b_0	EQU	$86
__local_lt__a_0	EQU	$87
__zpabi_gt_nested__b_1	EQU	$87
__zpabi_le__b_1	EQU	$87
__zpabi_lt__b_0	EQU	$88
__zpabi_lt__b_1	EQU	$89
__local_main__0	EQU	$8A
__local_main__4	EQU	$8E
__local_main__6	EQU	$90
__local_main__arr	EQU	$91
__local_main__nested_arr	EQU	$A5
__local_gt_nested__a_1	EQU	$F5
__local_le__a_1	EQU	$F5
__local_le__a_0	EQU	$F6

; @zp-link-meta-begin
; def ge params=__zpabi_ge__a_0,__zpabi_ge__a_1,__zpabi_ge__b_0,__zpabi_ge__b_1 locals=__local_ge__a_1,__local_ge__a_0 indirect=false in_cycle=false param_regs=A,X,,
; def ge_nested params=__zpabi_ge_nested__a_0,__zpabi_ge_nested__a_1,__zpabi_ge_nested__b_0,__zpabi_ge_nested__b_1 locals=__local_ge_nested__a_1,__local_ge_nested__a_0 indirect=false in_cycle=false param_regs=A,X,,
; def gt params=__zpabi_gt__a_0,__zpabi_gt__a_1,__zpabi_gt__b_0,__zpabi_gt__b_1 locals=__local_gt__a_1,__local_gt__a_0 indirect=false in_cycle=false param_regs=A,X,,
; def gt_nested params=__zpabi_gt_nested__a_0,__zpabi_gt_nested__a_1,__zpabi_gt_nested__b_0,__zpabi_gt_nested__b_1 locals=__local_gt_nested__a_1,__local_gt_nested__a_0 indirect=false in_cycle=false param_regs=A,X,,
; def le params=__zpabi_le__a_0,__zpabi_le__a_1,__zpabi_le__b_0,__zpabi_le__b_1 locals=__local_le__a_1,__local_le__a_0 indirect=false in_cycle=false param_regs=A,X,,
; def lt params=__zpabi_lt__a_0,__zpabi_lt__a_1,__zpabi_lt__b_0,__zpabi_lt__b_1 locals=__local_lt__a_1,__local_lt__a_0 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__arr,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__nested_arr,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39,__local_main__40,__local_main__41,__local_main__42,__local_main__43,__local_main__44,__local_main__45,__local_main__46,__local_main__47,__local_main__48,__local_main__49,__local_main__50,__local_main__51,__local_main__52,__local_main__53,__local_main__54,__local_main__55,__local_main__56,__local_main__57,__local_main__58,__local_main__59,__local_main__60,__local_main__61,__local_main__62,__local_main__63,__local_main__64,__local_main__65,__local_main__66,__local_main__67,__local_main__68,__local_main__69,__local_main__70,__local_main__71,__local_main__72,__local_main__73,__local_main__74,__local_main__75,__local_main__76,__local_main__77,__local_main__78,__local_main__79,__local_main__80,__local_main__81,__local_main__82,__local_main__83,__local_main__84,__local_main__85,__local_main__86,__local_main__87,__local_main__88,__local_main__89,__local_main__90,__local_main__91,__local_main__92,__local_main__93,__local_main__94,__local_main__95,__local_main__96,__local_main__97,__local_main__98,__local_main__99,__local_main__100,__local_main__101,__local_main__102,__local_main__103,__local_main__104,__local_main__105,__local_main__106 indirect=false in_cycle=false
; call main -> ge
; call main -> ge_nested
; call main -> gt
; call main -> gt_nested
; call main -> le
; call main -> lt
; share member=ge:__zpabi_ge__b_0,__zpabi_ge__b_1 holder=main live=main:ge,gt,lt writer=main
; share member=ge_nested:__zpabi_ge_nested__b_0,__zpabi_ge_nested__b_1 holder=main live=main:ge_nested,gt_nested forward=main>ge_nested writer=main
; share member=gt:__zpabi_gt__b_0,__zpabi_gt__b_1 holder=main live=main:ge,gt,le,lt forward=main>gt writer=main
; share member=lt:__zpabi_lt__b_0,__zpabi_lt__b_1 holder=main live=main:lt writer=main
; item gt section=.text local=false zero=false
; item gt@run1 section=.text local=true zero=false
; item gt@tail0 section=.text local=true zero=false
; item lt section=.text local=false zero=false
; item ge section=.text local=false zero=false
; item le section=.text local=false zero=false
; item gt_nested section=.text local=false zero=false
; item ge_nested section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

gt:
   SUBROUTINE

   CLC
   SBC   __zpabi_gt__b_0
   TXA
   SBC   __zpabi_gt__b_1
   LDA   #$00
   ROL
   STA   __local_gt__a_1
   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   JSR   gt@run1
   LDA   __local_gt__a_1
   JMP   gt@tail0

gt@run1:
   SUBROUTINE

   STA   HARGS+10
   STA   HARGS+11
   RTS

gt@tail0:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

lt:
   SUBROUTINE

   STA   __local_lt__a_0
   STX   __local_lt__a_1
   LDA   __zpabi_lt__b_0
   CLC
   SBC   __local_lt__a_0
   LDA   __zpabi_lt__b_1
   SBC   __local_lt__a_1
   LDA   #$00
   ROL
   STA   __local_lt__a_1
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   JSR   gt@run1
   LDA   __local_lt__a_1
   JMP   gt@tail0

ge:
   SUBROUTINE

   SEC
   SBC   __zpabi_ge__b_0
   TXA
   SBC   __zpabi_ge__b_1
   LDA   #$00
   ROL
   STA   __local_ge__a_1
   LDA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   JSR   gt@run1
   LDA   __local_ge__a_1
   JMP   gt@tail0

le:
   SUBROUTINE

   STA   __local_le__a_0
   STX   __local_le__a_1
   LDA   __zpabi_le__b_0
   SEC
   SBC   __local_le__a_0
   LDA   __zpabi_le__b_1
   SBC   __local_le__a_1
   LDA   #$00
   ROL
   STA   __local_le__a_1
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   JSR   gt@run1
   LDA   __local_le__a_1
   JMP   gt@tail0

gt_nested:
   SUBROUTINE

   CLC
   SBC   __zpabi_gt_nested__b_0
   TXA
   SBC   __zpabi_gt_nested__b_1
   LDA   #$00
   ROL
   STA   __local_gt_nested__a_1
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   JSR   gt@run1
   LDA   __local_gt_nested__a_1
   JMP   gt@tail0

ge_nested:
   SUBROUTINE

   SEC
   SBC   __zpabi_ge_nested__b_0
   TXA
   SBC   __zpabi_ge_nested__b_1
   LDA   #$00
   ROL
   STA   __local_ge_nested__a_1
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   JSR   gt@run1
   LDA   __local_ge_nested__a_1
   JMP   gt@tail0

main:
   SUBROUTINE

   LDA   #>__local_main__arr
   STA   __zpabi_ge__b_0+1
   LDA   #<__local_main__arr
   CLC
   ADC   #$04
   STA   __zpabi_ge__b_0
   BCC   .carry_inc@0
   INC   __zpabi_ge__b_1
.carry_inc@0:
   LDA   #>__local_main__arr
   STA   __zpabi_gt__b_0+1
   LDA   #<__local_main__arr
   CLC
   ADC   #$10
   STA   __zpabi_gt__b_0
   BCC   .carry_inc@1
   INC   __zpabi_gt__b_1
.carry_inc@1:
   LDX   __zpabi_ge__b_1
   LDA   __zpabi_ge__b_0
   JSR   gt
   JSR   main@run0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __zpabi_gt__b_0
   STA   __zpabi_lt__b_0
   LDA   __zpabi_gt__b_1
   STA   __zpabi_lt__b_1
   LDX   __zpabi_ge__b_1
   LDA   __zpabi_ge__b_0
   JSR   lt
   JSR   main@run0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   __zpabi_ge__b_1
   LDA   __zpabi_ge__b_0
   JSR   ge
   JSR   main@run0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __zpabi_ge__b_0
   STA   __zpabi_le__b_0
   LDA   __zpabi_ge__b_1
   STA   __zpabi_le__b_1
   LDX   __zpabi_gt__b_1
   LDA   __zpabi_gt__b_0
   JSR   le
   JSR   main@run0
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #>__local_main__arr
   STA   __local_main__6
   LDA   #<__local_main__arr
   CLC
   ADC   #$14
   STA   __local_main__4
   BCC   .carry_inc@2
   INC   __local_main__6
.carry_inc@2:
   LDX   __local_main__6
   JSR   gt
   JSR   main@run0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __zpabi_gt__b_0
   CLC
   ADC   #$04
   STA   __zpabi_gt__b_0
   BCC   .carry_inc@3
   INC   __zpabi_gt__b_1
.carry_inc@3:
   LDA   __local_main__6
   CMP   __zpabi_gt__b_1
   BNE   .jcmp_skip@12
   LDA   __local_main__4
   CMP   __zpabi_gt__b_0
   BEQ   .if_end@5
.jcmp_skip@12:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #>__local_main__nested_arr
   STA   __zpabi_lt__b_0+1
   LDA   #<(__local_main__nested_arr+68)
   STA   __zpabi_lt__b_0
   LDA   #>(__local_main__nested_arr+68)
   STA   __zpabi_lt__b_1
   LDX   #>(__local_main__nested_arr+72)
   LDA   #<(__local_main__nested_arr+72)
   JSR   lt
   JSR   main@run0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   #<(__local_main__nested_arr+68)
   STA   __zpabi_ge__b_0
   LDA   #>(__local_main__nested_arr+68)
   STA   __zpabi_ge__b_1
   LDX   #>(__local_main__nested_arr+72)
   LDA   #<(__local_main__nested_arr+72)
   JSR   ge
   JSR   main@run0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   #>__local_main__nested_arr
   STA   __zpabi_ge_nested__b_0+1
   LDA   #<__local_main__nested_arr
   CLC
   ADC   #$3C
   STA   __zpabi_ge_nested__b_0
   BCC   .carry_inc@4
   INC   __zpabi_ge_nested__b_1
.carry_inc@4:
   LDA   #>__local_main__nested_arr
   STA   __local_main__6
   LDA   #<__local_main__nested_arr
   CLC
   ADC   #$50
   STA   __local_main__4
   BCC   .carry_inc@5
   INC   __local_main__6
.carry_inc@5:
   LDX   #>__local_main__nested_arr
   LDA   #<__local_main__nested_arr
   JSR   ge_nested
   JSR   main@run0
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   __zpabi_ge_nested__b_0
   STA   __zpabi_gt_nested__b_0
   LDA   __zpabi_ge_nested__b_1
   STA   __zpabi_gt_nested__b_1
   LDX   __local_main__6
   LDA   __local_main__4
   JSR   gt_nested
   JSR   main@run0
   BNE   .if_end@9
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDA   __local_main__4
   SEC
   SBC   #$14
   STA   __local_main__0
   BCS   .carry_inc@6
   DEC   __local_main__6
.carry_inc@6:
   LDA   __zpabi_ge_nested__b_1
   CMP   __local_main__6
   BNE   .jcmp_skip@13
   LDA   __zpabi_ge_nested__b_0
   CMP   __local_main__0
   BEQ   .if_end@10
.jcmp_skip@13:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   RTS
