SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_test_non_constant__negative_7billion_0	EQU	$80
__zpabi_test_non_constant__negative_7billion_1	EQU	$81
__local_test_preserve_stack__arr_3	EQU	$82
__zpabi_test_non_constant__negative_7billion_2	EQU	$82
__local_test_preserve_stack__arr_2	EQU	$83
__zpabi_test_non_constant__negative_7billion_3	EQU	$83
__local_test_preserve_stack__arr_0	EQU	$84
__zpabi_test_non_constant__negative_7billion_4	EQU	$84
__local_test_preserve_stack__arr_1	EQU	$85
__zpabi_test_non_constant__negative_7billion_5	EQU	$85
__zpabi_test_non_constant__negative_7billion_6	EQU	$86
__zpabi_test_non_constant__negative_7billion_7	EQU	$87
__zpabi_test_non_constant__ptr_0	EQU	$88
__zpabi_test_non_constant__ptr_1	EQU	$89
__local_main__0	EQU	$8A
__local_main__i	EQU	$8C
__local_test_non_constant__arr_39	EQU	$8E
__local_test_partial__0	EQU	$8E
__local_test_type_conversion__ptr	EQU	$8E
__local_test_non_constant__arr_38	EQU	$8F
__local_test_partial__1	EQU	$8F
__local_test_type_conversion__arr_30	EQU	$8F
__local_test_non_constant__arr_37	EQU	$90
__local_test_partial__2	EQU	$90
__local_test_type_conversion__arr_29	EQU	$90
__local_test_non_constant__arr_36	EQU	$91
__local_test_partial__3	EQU	$91
__local_test_type_conversion__arr_28	EQU	$91
__local_test_non_constant__arr_35	EQU	$92
__local_test_partial__4	EQU	$92
__local_test_type_conversion__arr_27	EQU	$92
__local_test_non_constant__arr_34	EQU	$93
__local_test_partial__5	EQU	$93
__local_test_type_conversion__arr_26	EQU	$93
__local_test_non_constant__arr_33	EQU	$94
__local_test_partial__6	EQU	$94
__local_test_type_conversion__arr_25	EQU	$94
__local_test_non_constant__arr_32	EQU	$95
__local_test_partial__7	EQU	$95
__local_test_type_conversion__arr_24	EQU	$95
__local_test_non_constant__arr_31	EQU	$96
__local_test_partial__arr	EQU	$96
__local_test_non_constant__arr_30	EQU	$97
__local_test_non_constant__arr_29	EQU	$98
__local_test_non_constant__arr_28	EQU	$99
__local_test_non_constant__arr_27	EQU	$9A
__local_test_non_constant__arr_26	EQU	$9B
__local_test_non_constant__arr_25	EQU	$9C
__local_test_non_constant__arr_24	EQU	$9D
__local_test_non_constant__arr_23	EQU	$9E
__local_test_type_conversion__arr_15	EQU	$9E
__local_test_non_constant__arr_22	EQU	$9F
__local_test_non_constant__arr_21	EQU	$A0
__local_test_non_constant__arr_20	EQU	$A1
__local_test_non_constant__arr_19	EQU	$A2
__local_test_non_constant__arr_18	EQU	$A3
__local_test_non_constant__arr_17	EQU	$A4
__local_test_type_conversion__arr_9	EQU	$A4
__local_test_non_constant__arr_16	EQU	$A5
__local_test_type_conversion__arr_8	EQU	$A5
__local_test_non_constant__arr_15	EQU	$A6

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__i,__local_main__3 indirect=false in_cycle=false
; def test_non_constant params=__zpabi_test_non_constant__negative_7billion_0,__zpabi_test_non_constant__negative_7billion_1,__zpabi_test_non_constant__negative_7billion_2,__zpabi_test_non_constant__negative_7billion_3,__zpabi_test_non_constant__negative_7billion_4,__zpabi_test_non_constant__negative_7billion_5,__zpabi_test_non_constant__negative_7billion_6,__zpabi_test_non_constant__negative_7billion_7,__zpabi_test_non_constant__ptr_0,__zpabi_test_non_constant__ptr_1 locals=__local_test_non_constant__arr_39,__local_test_non_constant__arr_38,__local_test_non_constant__arr_37,__local_test_non_constant__arr_36,__local_test_non_constant__arr_35,__local_test_non_constant__arr_34,__local_test_non_constant__arr_33,__local_test_non_constant__arr_32,__local_test_non_constant__arr_31,__local_test_non_constant__arr_30,__local_test_non_constant__arr_29,__local_test_non_constant__arr_28,__local_test_non_constant__arr_27,__local_test_non_constant__arr_26,__local_test_non_constant__arr_25,__local_test_non_constant__arr_24,__local_test_non_constant__arr_23,__local_test_non_constant__arr_22,__local_test_non_constant__arr_21,__local_test_non_constant__arr_20,__local_test_non_constant__arr_19,__local_test_non_constant__arr_18,__local_test_non_constant__arr_17,__local_test_non_constant__arr_16,__local_test_non_constant__arr_15,__local_test_non_constant__arr_14,__local_test_non_constant__arr_13,__local_test_non_constant__arr_12,__local_test_non_constant__arr_11,__local_test_non_constant__arr_10,__local_test_non_constant__arr_9,__local_test_non_constant__arr_8,__local_test_non_constant__arr_7,__local_test_non_constant__arr_6,__local_test_non_constant__arr_5,__local_test_non_constant__arr_4,__local_test_non_constant__arr_3,__local_test_non_constant__arr_2,__local_test_non_constant__arr_1,__local_test_non_constant__arr_0,__local_test_non_constant__0,__local_test_non_constant__1,__local_test_non_constant__2,__local_test_non_constant__3,__local_test_non_constant__4,__local_test_non_constant__5,__local_test_non_constant__6,__local_test_non_constant__7 indirect=false in_cycle=false changes=__zpabi_test_non_constant__negative_7billion_0,__zpabi_test_non_constant__negative_7billion_1,__zpabi_test_non_constant__negative_7billion_2,__zpabi_test_non_constant__negative_7billion_3,__zpabi_test_non_constant__negative_7billion_4,__zpabi_test_non_constant__negative_7billion_5,__zpabi_test_non_constant__negative_7billion_6,__zpabi_test_non_constant__negative_7billion_7
; def test_partial params= locals=__local_test_partial__0,__local_test_partial__1,__local_test_partial__2,__local_test_partial__3,__local_test_partial__4,__local_test_partial__5,__local_test_partial__6,__local_test_partial__7,__local_test_partial__arr,__local_test_partial__9,__local_test_partial__10,__local_test_partial__11,__local_test_partial__12,__local_test_partial__13,__local_test_partial__14,__local_test_partial__15,__local_test_partial__16,__local_test_partial__17,__local_test_partial__18,__local_test_partial__19,__local_test_partial__20,__local_test_partial__21,__local_test_partial__22,__local_test_partial__23,__local_test_partial__24,__local_test_partial__25,__local_test_partial__26,__local_test_partial__27,__local_test_partial__28,__local_test_partial__29,__local_test_partial__30,__local_test_partial__31,__local_test_partial__32,__local_test_partial__33,__local_test_partial__34,__local_test_partial__35,__local_test_partial__36,__local_test_partial__37,__local_test_partial__38,__local_test_partial__39,__local_test_partial__40,__local_test_partial__41,__local_test_partial__42,__local_test_partial__43,__local_test_partial__44,__local_test_partial__45,__local_test_partial__46,__local_test_partial__47 indirect=false in_cycle=false
; def test_preserve_stack params= locals=__local_test_preserve_stack__arr_5,__local_test_preserve_stack__arr_4,__local_test_preserve_stack__arr_3,__local_test_preserve_stack__arr_2,__local_test_preserve_stack__arr_0,__local_test_preserve_stack__arr_1 indirect=false in_cycle=false
; def test_simple params= locals=__local_test_simple__arr_23,__local_test_simple__arr_22,__local_test_simple__arr_21,__local_test_simple__arr_20,__local_test_simple__arr_19,__local_test_simple__arr_18,__local_test_simple__arr_17,__local_test_simple__arr_16,__local_test_simple__arr_15,__local_test_simple__arr_14,__local_test_simple__arr_13,__local_test_simple__arr_12,__local_test_simple__arr_11,__local_test_simple__arr_10,__local_test_simple__arr_9,__local_test_simple__arr_8,__local_test_simple__arr_7,__local_test_simple__arr_6,__local_test_simple__arr_5,__local_test_simple__arr_4,__local_test_simple__arr_3,__local_test_simple__arr_2,__local_test_simple__arr_1,__local_test_simple__arr_0 indirect=false in_cycle=false
; def test_type_conversion params=__zpabi_test_type_conversion__ptr_0,__zpabi_test_type_conversion__ptr_1 locals=__local_test_type_conversion__ptr,__local_test_type_conversion__arr_30,__local_test_type_conversion__arr_29,__local_test_type_conversion__arr_28,__local_test_type_conversion__arr_27,__local_test_type_conversion__arr_26,__local_test_type_conversion__arr_25,__local_test_type_conversion__arr_24,__local_test_type_conversion__arr_23,__local_test_type_conversion__arr_22,__local_test_type_conversion__arr_21,__local_test_type_conversion__arr_20,__local_test_type_conversion__arr_19,__local_test_type_conversion__arr_18,__local_test_type_conversion__arr_17,__local_test_type_conversion__arr_16,__local_test_type_conversion__arr_15,__local_test_type_conversion__arr_14,__local_test_type_conversion__arr_13,__local_test_type_conversion__arr_12,__local_test_type_conversion__arr_11,__local_test_type_conversion__arr_10,__local_test_type_conversion__arr_9,__local_test_type_conversion__arr_8,__local_test_type_conversion__arr_7,__local_test_type_conversion__arr_6,__local_test_type_conversion__arr_5,__local_test_type_conversion__arr_4,__local_test_type_conversion__arr_3,__local_test_type_conversion__arr_2,__local_test_type_conversion__arr_1,__local_test_type_conversion__arr_0 indirect=false in_cycle=false param_regs=A,X
; def three params= locals= indirect=false in_cycle=false
; call main -> test_non_constant
; call main -> test_partial
; call main -> test_preserve_stack
; call main -> test_simple
; call main -> test_type_conversion
; call test_non_constant -> three
; call test_preserve_stack -> three
; item test_simple section=.text local=false zero=false
; item test_simple@tail0 section=.text local=true zero=false
; item test_partial section=.text local=false zero=false
; item test_non_constant section=.text local=false zero=false
; item three section=.text local=false zero=false
; item test_type_conversion section=.text local=false zero=false
; item test_preserve_stack section=.text local=false zero=false
; item main section=.text local=false zero=false
; item global_one section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

test_simple:
   SUBROUTINE

   LDA   #$00
   LDY   #$00
   CMP   #$00
   BNE   .and_false@0
   LDA   #$FF
   CMP   #$FF
   BNE   .and_false@0
   TYA
   BNE   .and_false@0
   LDA   #$7F
   CMP   #$7F
   BNE   .and_false@0
   LDA   #$FF
   CMP   #$FF
   BNE   .and_false@0
   TYA
   BNE   .and_false@0
   LDA   #$64
   CMP   #$64
   BNE   .and_false@0
   JMP   test_simple@tail0
.and_false@0:
   TYA
   LDX   #$00
   RTS

test_simple@tail0:
   SUBROUTINE

   LDA   #$01
   LDX   #$00
   RTS

test_partial:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_partial__arr
   LDY   #$00
   STY   __local_test_partial__arr+1
   STY   __local_test_partial__arr+2
   STY   __local_test_partial__arr+3
   STY   __local_test_partial__arr+4
   STA   __local_test_partial__arr+5
   LDA   #$F0
   STA   __local_test_partial__arr+6
   LDA   #$3F
   STA   __local_test_partial__arr+7
   TYA
   STA   __local_test_partial__arr+8
   STY   __local_test_partial__arr+9
   STY   __local_test_partial__arr+10
   STA   __local_test_partial__arr+11
   LDA   #$B0
   STA   __local_test_partial__arr+12
   LDA   #$C4
   STA   __local_test_partial__arr+13
   LDA   #$32
   STA   __local_test_partial__arr+14
   LDA   #$41
   STA   __local_test_partial__arr+15
   LDA   #$18
   STA   __local_test_partial__4
.clear@4:
   DEC   __local_test_partial__4
   LDA   __local_test_partial__4
   CLC
   ADC   #<__local_test_partial__arr
   STA   __local_test_partial__0
   LDA   #$00
   ADC   #>__local_test_partial__arr
   STA   __local_test_partial__1
   LDA   #$00
   LDY   #$10
   STA   (__local_test_partial__0),Y
   LDA   __local_test_partial__4
   BNE   .clear@4
   LDA   __local_test_partial__arr
   STA   __local_test_partial__7
   LDA   __local_test_partial__arr+1
   STA   __local_test_partial__6
   LDA   __local_test_partial__arr+2
   STA   __local_test_partial__5
   LDA   __local_test_partial__arr+3
   STA   __local_test_partial__4
   LDA   __local_test_partial__arr+4
   STA   __local_test_partial__3
   LDA   __local_test_partial__arr+5
   STA   __local_test_partial__2
   LDA   __local_test_partial__arr+6
   STA   __local_test_partial__1
   LDA   __local_test_partial__arr+7
   CMP   #$3F
   BEQ   .lb_skip@16
   JMP   .and_false@5
.lb_skip@16:
   LDA   __local_test_partial__1
   CMP   #$F0
   BEQ   .lb_skip@15
   JMP   .and_false@5
.lb_skip@15:
   LDA   __local_test_partial__2
   BEQ   .lb_skip@14
   JMP   .and_false@5
.lb_skip@14:
   LDA   __local_test_partial__3
   BEQ   .lb_skip@13
   JMP   .and_false@5
.lb_skip@13:
   LDA   __local_test_partial__4
   BEQ   .lb_skip@12
   JMP   .and_false@5
.lb_skip@12:
   LDA   __local_test_partial__5
   BEQ   .lb_skip@11
   JMP   .and_false@5
.lb_skip@11:
   LDA   __local_test_partial__6
   BEQ   .lb_skip@10
   JMP   .and_false@5
.lb_skip@10:
   LDA   __local_test_partial__7
   BEQ   .lb_skip@9
   JMP   .and_false@5
.lb_skip@9:
   LDA   __local_test_partial__arr+8
   STA   __local_test_partial__7
   LDA   __local_test_partial__arr+9
   STA   __local_test_partial__6
   LDA   __local_test_partial__arr+10
   STA   __local_test_partial__5
   LDA   __local_test_partial__arr+11
   STA   __local_test_partial__4
   LDA   __local_test_partial__arr+12
   STA   __local_test_partial__3
   LDA   __local_test_partial__arr+13
   STA   __local_test_partial__2
   LDA   __local_test_partial__arr+14
   STA   __local_test_partial__1
   LDA   __local_test_partial__arr+15
   CMP   #$41
   BEQ   .lb_skip@8
   JMP   .and_false@5
.lb_skip@8:
   LDA   __local_test_partial__1
   CMP   #$32
   BEQ   .lb_skip@7
   JMP   .and_false@5
.lb_skip@7:
   LDA   __local_test_partial__2
   CMP   #$C4
   BEQ   .lb_skip@6
   JMP   .and_false@5
.lb_skip@6:
   LDA   __local_test_partial__3
   CMP   #$B0
   BEQ   .lb_skip@5
   JMP   .and_false@5
.lb_skip@5:
   LDA   __local_test_partial__4
   BEQ   .lb_skip@4
   JMP   .and_false@5
.lb_skip@4:
   LDA   __local_test_partial__5
   BEQ   .lb_skip@3
   JMP   .and_false@5
.lb_skip@3:
   LDA   __local_test_partial__6
   BEQ   .lb_skip@2
   JMP   .and_false@5
.lb_skip@2:
   LDA   __local_test_partial__7
   BEQ   .lb_skip@1
   JMP   .and_false@5
.lb_skip@1:
   LDA   __local_test_partial__arr+16
   LDX   __local_test_partial__arr+17
   STX   __local_test_partial__6
   LDX   __local_test_partial__arr+18
   STX   __local_test_partial__5
   LDX   __local_test_partial__arr+19
   STX   __local_test_partial__4
   LDX   __local_test_partial__arr+20
   STX   __local_test_partial__3
   LDX   __local_test_partial__arr+21
   STX   __local_test_partial__2
   LDX   __local_test_partial__arr+22
   STX   __local_test_partial__1
   LDX   __local_test_partial__arr+23
   STX   __local_test_partial__0
   ORA   __local_test_partial__6
   ORA   __local_test_partial__5
   ORA   __local_test_partial__4
   ORA   __local_test_partial__3
   ORA   __local_test_partial__2
   ORA   __local_test_partial__1
   ORA   __local_test_partial__0
   BEQ   .lb_skip@0
   JMP   .and_false@5
.lb_skip@0:
   LDA   __local_test_partial__arr+24
   LDX   __local_test_partial__arr+25
   STX   __local_test_partial__6
   LDX   __local_test_partial__arr+26
   STX   __local_test_partial__5
   LDX   __local_test_partial__arr+27
   STX   __local_test_partial__4
   LDX   __local_test_partial__arr+28
   STX   __local_test_partial__3
   LDX   __local_test_partial__arr+29
   STX   __local_test_partial__2
   LDX   __local_test_partial__arr+30
   STX   __local_test_partial__1
   LDX   __local_test_partial__arr+31
   STX   __local_test_partial__0
   ORA   __local_test_partial__6
   ORA   __local_test_partial__5
   ORA   __local_test_partial__4
   ORA   __local_test_partial__3
   ORA   __local_test_partial__2
   ORA   __local_test_partial__1
   ORA   __local_test_partial__0
   BNE   .and_false@5
   LDA   __local_test_partial__arr+32
   LDX   __local_test_partial__arr+33
   STX   __local_test_partial__6
   LDX   __local_test_partial__arr+34
   STX   __local_test_partial__5
   LDX   __local_test_partial__arr+35
   STX   __local_test_partial__4
   LDX   __local_test_partial__arr+36
   STX   __local_test_partial__3
   LDX   __local_test_partial__arr+37
   STX   __local_test_partial__2
   LDX   __local_test_partial__arr+38
   STX   __local_test_partial__1
   LDX   __local_test_partial__arr+39
   STX   __local_test_partial__0
   ORA   __local_test_partial__6
   ORA   __local_test_partial__5
   ORA   __local_test_partial__4
   ORA   __local_test_partial__3
   ORA   __local_test_partial__2
   ORA   __local_test_partial__1
   ORA   __local_test_partial__0
   BNE   .and_false@5
   JMP   test_simple@tail0
.and_false@5:
   LDA   #$00
   LDX   #$00
   RTS

test_non_constant:
   SUBROUTINE

   LDA   #$01
   LDY   #$00
   STA   (__zpabi_test_non_constant__ptr_0),Y
   LDA   #$00
   INY
   STA   (__zpabi_test_non_constant__ptr_0),Y
   JSR   three
   STA   __local_test_non_constant__arr_38
   TXA
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   LDA   __zpabi_test_non_constant__negative_7billion_0
   STA   HARGS
   LDA   __zpabi_test_non_constant__negative_7billion_1
   STA   HARGS+1
   LDA   __zpabi_test_non_constant__negative_7billion_2
   STA   HARGS+2
   LDA   __zpabi_test_non_constant__negative_7billion_3
   STA   HARGS+3
   LDA   __zpabi_test_non_constant__negative_7billion_4
   STA   HARGS+4
   LDA   __zpabi_test_non_constant__negative_7billion_5
   STA   HARGS+5
   LDA   __zpabi_test_non_constant__negative_7billion_6
   STA   HARGS+6
   LDA   __zpabi_test_non_constant__negative_7billion_7
   STA   HARGS+7
   LDA   __local_test_non_constant__arr_38
   STA   HARGS+8
   STX   HARGS+9
   JSR   mul64
   LDA   HARGS+16
   STA   __local_test_non_constant__arr_24
   LDA   HARGS+17
   STA   __local_test_non_constant__arr_25
   LDA   HARGS+18
   STA   __local_test_non_constant__arr_26
   LDA   HARGS+19
   STA   __local_test_non_constant__arr_27
   LDA   HARGS+20
   STA   __local_test_non_constant__arr_28
   LDA   HARGS+21
   STA   __local_test_non_constant__arr_29
   LDA   HARGS+22
   STA   __local_test_non_constant__arr_30
   LDA   HARGS+23
   STA   __local_test_non_constant__arr_31
   JSR   three
   STA   __local_test_non_constant__arr_38
   TXA
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_test_non_constant__arr_38
   STA   HARGS
   STX   HARGS+1
   LDA   #$07
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+11
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_test_non_constant__arr_15
   LDY   #$00
   LDA   (__zpabi_test_non_constant__ptr_0),Y
   STA   __local_test_non_constant__arr_32
   INY
   LDA   (__zpabi_test_non_constant__ptr_0),Y
   STA   __local_test_non_constant__arr_33
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_test_non_constant__arr_34
   STA   __local_test_non_constant__arr_35
   STA   __local_test_non_constant__arr_36
   STA   __local_test_non_constant__arr_37
   STA   __local_test_non_constant__arr_38
   STA   __local_test_non_constant__arr_39
   SEC
   LDA   #$00
   SBC   __local_test_non_constant__arr_32
   STA   __local_test_non_constant__arr_16
   LDA   #$00
   SBC   __local_test_non_constant__arr_33
   STA   __local_test_non_constant__arr_17
   LDA   #$00
   SBC   __local_test_non_constant__arr_34
   STA   __local_test_non_constant__arr_18
   LDA   #$00
   SBC   __local_test_non_constant__arr_35
   STA   __local_test_non_constant__arr_19
   LDA   #$00
   SBC   __local_test_non_constant__arr_36
   STA   __local_test_non_constant__arr_20
   LDA   #$00
   SBC   __local_test_non_constant__arr_37
   STA   __local_test_non_constant__arr_21
   LDA   #$00
   SBC   __local_test_non_constant__arr_38
   STA   __local_test_non_constant__arr_22
   LDA   #$00
   SBC   __local_test_non_constant__arr_39
   STA   __local_test_non_constant__arr_23
   LDA   __zpabi_test_non_constant__negative_7billion_0
   ORA   __zpabi_test_non_constant__negative_7billion_1
   ORA   __zpabi_test_non_constant__negative_7billion_2
   ORA   __zpabi_test_non_constant__negative_7billion_3
   ORA   __zpabi_test_non_constant__negative_7billion_4
   ORA   __zpabi_test_non_constant__negative_7billion_5
   ORA   __zpabi_test_non_constant__negative_7billion_6
   ORA   __zpabi_test_non_constant__negative_7billion_7
   BEQ   .cond_else@13
   LDA   #$02
   STA   __local_test_non_constant__arr_32
   LDA   #$00
   BEQ   .sx_done@9
.cond_else@13:
   LDA   #$03
   STA   __local_test_non_constant__arr_32
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_test_non_constant__arr_34
   STA   __local_test_non_constant__arr_35
   STA   __local_test_non_constant__arr_36
   STA   __local_test_non_constant__arr_37
   STA   __local_test_non_constant__arr_38
   STA   __local_test_non_constant__arr_39
   LDA   __local_test_non_constant__arr_24
   CLC
   ADC   __local_test_non_constant__arr_32
   STA   __local_test_non_constant__arr_24
   LDA   __local_test_non_constant__arr_25
   ADC   #$00
   STA   __local_test_non_constant__arr_25
   LDA   __local_test_non_constant__arr_26
   ADC   __local_test_non_constant__arr_34
   STA   __local_test_non_constant__arr_26
   LDA   __local_test_non_constant__arr_27
   ADC   __local_test_non_constant__arr_35
   STA   __local_test_non_constant__arr_27
   LDA   __local_test_non_constant__arr_28
   ADC   __local_test_non_constant__arr_36
   STA   __local_test_non_constant__arr_28
   LDA   __local_test_non_constant__arr_29
   ADC   __local_test_non_constant__arr_37
   STA   __local_test_non_constant__arr_29
   LDA   __local_test_non_constant__arr_30
   ADC   __local_test_non_constant__arr_38
   STA   __local_test_non_constant__arr_30
   LDA   __local_test_non_constant__arr_31
   ADC   __local_test_non_constant__arr_39
   STA   __local_test_non_constant__arr_31
   LDY   #$00
   LDA   __zpabi_test_non_constant__negative_7billion_7
   CMP   #$FF
   BEQ   .lb_skip@27
   JMP   .and_false@15
.lb_skip@27:
   LDA   __zpabi_test_non_constant__negative_7billion_6
   CMP   #$FF
   BEQ   .lb_skip@26
   JMP   .and_false@15
.lb_skip@26:
   LDA   __zpabi_test_non_constant__negative_7billion_5
   CMP   #$FF
   BEQ   .lb_skip@25
   JMP   .and_false@15
.lb_skip@25:
   LDA   __zpabi_test_non_constant__negative_7billion_4
   CMP   #$FF
   BEQ   .lb_skip@24
   JMP   .and_false@15
.lb_skip@24:
   LDA   __zpabi_test_non_constant__negative_7billion_3
   CMP   #$D6
   BEQ   .lb_skip@23
   JMP   .and_false@15
.lb_skip@23:
   LDA   __zpabi_test_non_constant__negative_7billion_2
   CMP   #$46
   BEQ   .lb_skip@22
   JMP   .and_false@15
.lb_skip@22:
   LDA   __zpabi_test_non_constant__negative_7billion_1
   CMP   #$D9
   BEQ   .lb_skip@21
   JMP   .and_false@15
.lb_skip@21:
   LDA   __zpabi_test_non_constant__negative_7billion_0
   BEQ   .lb_skip@20
   JMP   .and_false@15
.lb_skip@20:
   LDA   __local_test_non_constant__arr_15
   BEQ   .lb_skip@19
   JMP   .and_false@15
.lb_skip@19:
   LDA   HARGS+11
   BEQ   .lb_skip@18
   JMP   .and_false@15
.lb_skip@18:
   LDA   HARGS+10
   BEQ   .lb_skip@17
   JMP   .and_false@15
.lb_skip@17:
   LDA   HARGS+9
   BNE   .and_false@15
   LDA   HARGS+8
   CMP   #$15
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_23
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_22
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_21
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_20
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_19
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_18
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_17
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_16
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_31
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_30
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_29
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_28
   CMP   #$FF
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_27
   CMP   #$82
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_26
   CMP   #$D4
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_25
   CMP   #$8B
   BNE   .and_false@15
   LDA   __local_test_non_constant__arr_24
   CMP   #$02
   BNE   .and_false@15
   TYA
   BNE   .and_false@15
   JMP   test_simple@tail0
.and_false@15:
   TYA
   LDX   #$00
   RTS

three:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   RTS

test_type_conversion:
   SUBROUTINE

   STA   __local_test_type_conversion__ptr
   STX   __local_test_type_conversion__arr_30
   LDA   #$9C
   LDY   #$00
   STA   (__local_test_type_conversion__ptr),Y
   LDA   #$FF
   INY
   STA   (__local_test_type_conversion__ptr),Y
   DEY
   LDA   (__local_test_type_conversion__ptr),Y
   STA   __local_test_type_conversion__arr_8
   INY
   LDA   (__local_test_type_conversion__ptr),Y
   STA   __local_test_type_conversion__arr_9
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_test_type_conversion__arr_15
   LDA   #$00
   LDY   #$00
   SEC
   SBC   global_one
   STA   __local_test_type_conversion__arr_24
   TYA
   SBC   global_one+1
   STA   __local_test_type_conversion__arr_25
   TYA
   SBC   global_one+2
   STA   __local_test_type_conversion__arr_26
   TYA
   SBC   global_one+3
   STA   __local_test_type_conversion__arr_27
   TYA
   SBC   global_one+4
   STA   __local_test_type_conversion__arr_28
   TYA
   SBC   global_one+5
   STA   __local_test_type_conversion__arr_29
   TYA
   SBC   global_one+6
   STA   __local_test_type_conversion__arr_30
   TYA
   SBC   global_one+7
   STA   __local_test_type_conversion__ptr
   TYA
   BNE   .and_false@23
   LDA   #$3B
   CMP   #$3B
   BNE   .and_false@23
   LDA   #$9A
   CMP   #$9A
   BNE   .and_false@23
   LDA   #$CA
   CMP   #$CA
   BNE   .and_false@23
   TYA
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_15
   BNE   .and_false@23
   CMP   #$FF
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_9
   CMP   #$FF
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_8
   CMP   #$9C
   BNE   .and_false@23
   TYA
   BNE   .and_false@23
   LDA   #$FF
   BNE   .and_false@23
   CMP   #$FF
   BNE   .and_false@23
   LDA   __local_test_type_conversion__ptr
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_30
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_29
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_28
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_27
   CMP   #$FF
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_26
   CMP   #$FF
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_25
   CMP   #$FF
   BNE   .and_false@23
   LDA   __local_test_type_conversion__arr_24
   CMP   #$FF
   BNE   .and_false@23
   JMP   test_simple@tail0
.and_false@23:
   TYA
   LDX   #$00
   RTS

test_preserve_stack:
   SUBROUTINE

   LDA   global_one
   STA   __local_test_preserve_stack__arr_0
   LDA   global_one+1
   ASL   __local_test_preserve_stack__arr_0
   ROL
   STA   __local_test_preserve_stack__arr_1
   JSR   three
   CLC
   ADC   global_one
   STA   __local_test_preserve_stack__arr_2
   TXA
   ADC   global_one+1
   STA   __local_test_preserve_stack__arr_3
   LDA   __local_test_preserve_stack__arr_1
   BNE   .and_false@31
   LDA   __local_test_preserve_stack__arr_0
   CMP   #$02
   BNE   .and_false@31
   LDA   __local_test_preserve_stack__arr_3
   BNE   .and_false@31
   LDA   __local_test_preserve_stack__arr_2
   CMP   #$04
   BNE   .and_false@31
   LDA   #$00
   BNE   .and_false@31
   JMP   test_simple@tail0
.and_false@31:
   LDA   #$00
   LDX   #$00
   RTS

main:
   SUBROUTINE

   JSR   test_simple
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@35
   LDX   #$00
   LDA   #$01
   RTS
.if_end@35:
   JSR   test_partial
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@36
   LDX   #$00
   LDA   #$02
   RTS
.if_end@36:
   LDA   #$00
   STA   __local_main__i
   STA   __local_main__i+1
   STA   __zpabi_test_non_constant__negative_7billion_0
   LDA   #$D9
   STA   __zpabi_test_non_constant__negative_7billion_1
   LDA   #$46
   STA   __zpabi_test_non_constant__negative_7billion_2
   LDA   #$D6
   STA   __zpabi_test_non_constant__negative_7billion_3
   LDA   #$FF
   STA   __zpabi_test_non_constant__negative_7billion_4
   STA   __zpabi_test_non_constant__negative_7billion_5
   STA   __zpabi_test_non_constant__negative_7billion_6
   STA   __zpabi_test_non_constant__negative_7billion_7
   LDA   #<__local_main__i
   STA   __zpabi_test_non_constant__ptr_0
   LDA   #>__local_main__i
   STA   __zpabi_test_non_constant__ptr_1
   JSR   test_non_constant
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@37
   LDX   #$00
   LDA   #$03
   RTS
.if_end@37:
   LDX   #>__local_main__i
   LDA   #<__local_main__i
   JSR   test_type_conversion
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@38
   LDX   #$00
   LDA   #$04
   RTS
.if_end@38:
   JSR   test_preserve_stack
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@39
   LDX   #$00
   LDA   #$05
   RTS
.if_end@39:
   LDX   #$00
   TXA
   RTS

global_one:
   DC.L  $00000001
   DC.L  $00000000
