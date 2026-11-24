SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_add_chars__c2	EQU	$80
__local_test_for_loop_char__s	EQU	$80
__zpabi_and_char__i_0	EQU	$80
__zpabi_sub_char_from_pointer__idx	EQU	$80
__zpabi_subscript_char__idx	EQU	$80
__local_le__c1	EQU	$81
__local_test_for_loop_char__counter	EQU	$81
__zpabi_and_char__i_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_main__2	EQU	$84
__local_main__3	EQU	$85
__local_main__arr	EQU	$86
__local_sub_char_from_pointer__ptr_0	EQU	$8E
__local_subscript_char__0	EQU	$8E
__local_sub_char_from_pointer__0	EQU	$8F
__local_subscript_char__1	EQU	$8F
__local_sub_char_from_pointer__1	EQU	$90
__local_subscript_char__ptr_0	EQU	$90

; @zp-link-meta-begin
; def add_chars params=__zpabi_add_chars__c1,__zpabi_add_chars__c2 locals=__local_add_chars__c2,__local_add_chars__c1 indirect=false in_cycle=false param_regs=A,X
; def and_char params=__zpabi_and_char__c1,__zpabi_and_char__i_0,__zpabi_and_char__i_1 locals=__local_and_char__c1 indirect=false in_cycle=false param_regs=A,,
; def divide_chars params=__zpabi_divide_chars__c1,__zpabi_divide_chars__c2 locals=__local_divide_chars__c2 indirect=false in_cycle=false param_regs=A,X
; def le params=__zpabi_le__c1,__zpabi_le__c2 locals=__local_le__c2,__local_le__c1 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__arr,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; def or_char params=__zpabi_or_char__c1,__zpabi_or_char__c2 locals=__local_or_char__c2,__local_or_char__c1 indirect=false in_cycle=false param_regs=A,X
; def sub_char_from_pointer params=__zpabi_sub_char_from_pointer__ptr_0,__zpabi_sub_char_from_pointer__ptr_1,__zpabi_sub_char_from_pointer__idx locals=__local_sub_char_from_pointer__ptr_0,__local_sub_char_from_pointer__0,__local_sub_char_from_pointer__1,__local_sub_char_from_pointer__ptr_1 indirect=false in_cycle=false param_regs=A,X,
; def subscript_char params=__zpabi_subscript_char__ptr_0,__zpabi_subscript_char__ptr_1,__zpabi_subscript_char__idx locals=__local_subscript_char__0,__local_subscript_char__1,__local_subscript_char__ptr_0,__local_subscript_char__ptr_1 indirect=false in_cycle=false param_regs=A,X,
; def test_for_loop_char params= locals=__local_test_for_loop_char__s,__local_test_for_loop_char__counter indirect=false in_cycle=false
; call main -> add_chars
; call main -> and_char
; call main -> divide_chars
; call main -> le
; call main -> or_char
; call main -> sub_char_from_pointer
; call main -> subscript_char
; call main -> test_for_loop_char
; item add_chars section=.text local=false zero=false
; item divide_chars section=.text local=false zero=false
; item le section=.text local=false zero=false
; item subscript_char section=.text local=false zero=false
; item sub_char_from_pointer section=.text local=false zero=false
; item and_char section=.text local=false zero=false
; item or_char section=.text local=false zero=false
; item test_for_loop_char section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=4
; @zp-link-meta-end

add_chars:
   SUBROUTINE

   STX   __local_add_chars__c2
   CLC
   ADC   __local_add_chars__c2
   STA   __local_add_chars__c2
   LDA   #$00
   ADC   #$00
   TAX
   LDA   __local_add_chars__c2
   RTS

divide_chars:
   SUBROUTINE

   STA   HARGS
   STX   HARGS+1
   JSR   udivmod8
   LDA   HARGS+2
   LDX   #$00
   RTS

le:
   SUBROUTINE

   STA   __local_le__c1
   TXA
   SEC
   SBC   __local_le__c1
   LDA   #$00
   ROL
   LDX   #$00
   RTS

subscript_char:
   SUBROUTINE

   STA   __local_subscript_char__ptr_0
   LDA   __zpabi_subscript_char__idx
   ASL
   STA   __local_subscript_char__1
   LDA   #$00
   ROL
   STA   __local_subscript_char__0
   LDA   __local_subscript_char__ptr_0
   CLC
   ADC   __local_subscript_char__1
   STA   __local_subscript_char__1
   TXA
   ADC   __local_subscript_char__0
   STA   __local_subscript_char__ptr_0
   LDY   #$01
   LDA   (__local_subscript_char__1),Y
   TAX
   DEY
   LDA   (__local_subscript_char__1),Y
   RTS

sub_char_from_pointer:
   SUBROUTINE

   STA   __local_sub_char_from_pointer__ptr_0
   LDA   __zpabi_sub_char_from_pointer__idx
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_sub_char_from_pointer__1
   LDA   __zpabi_sub_char_from_pointer__idx
   ASL
   STA   __local_sub_char_from_pointer__0
   ROL   __local_sub_char_from_pointer__1
   LDA   __local_sub_char_from_pointer__ptr_0
   SEC
   SBC   __local_sub_char_from_pointer__0
   STA   __local_sub_char_from_pointer__ptr_0
   TXA
   SBC   __local_sub_char_from_pointer__1
   TAX
   LDA   __local_sub_char_from_pointer__ptr_0
   RTS

and_char:
   SUBROUTINE

   ORA   #$00
   BEQ   .and_false@0
   LDA   __zpabi_and_char__i_0
   ORA   __zpabi_and_char__i_1
   BEQ   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
   LDX   #$00
   RTS

or_char:
   SUBROUTINE

   ORA   #$00
   BNE   .or_true@2
   TXA
   BNE   .or_true@2
   LDA   #$00
   LDX   #$00
   RTS
.or_true@2:
   LDA   #$01
   LDX   #$00
   RTS

test_for_loop_char:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_for_loop_char__counter
   LDX   #$00
   LDA   #$7F
   STA   __local_test_for_loop_char__s
   JMP   .loop@0_start
.test_for_loop_char@rot@0:
   BEQ   .loop@0_break
   INC   __local_test_for_loop_char__counter
   BNE   .inc_done@0
   INX
.inc_done@0:
   DEC   __local_test_for_loop_char__s
.loop@0_start:
   LDA   __local_test_for_loop_char__s
   BPL   .test_for_loop_char@rot@0
.loop@0_break:
   CPX   #$00
   BNE   .thread@0
   LDA   __local_test_for_loop_char__counter
   CMP   #$7F
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDX   #$04
   LDA   #$08
   JSR   add_chars
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@5
   LDA   __local_main__1
   CMP   #$0C
   BEQ   .if_end@4
.jcmp_skip@5:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@4:
   LDX   #$19
   LDA   #$FA
   JSR   divide_chars
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@5
.jcmp_skip@6:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@5:
   LDX   #$04
   LDA   #$08
   JSR   le
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$03
   RTS
.if_end@6:
   LDX   #$04
   TXA
   JSR   le
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$04
   RTS
.if_end@7:
   LDA   #$0B
   STA   __local_main__arr
   LDA   #$00
   STA   __local_main__arr+1
   LDY   #$0C
   STY   __local_main__arr+2
   STA   __local_main__arr+3
   INY
   STY   __local_main__arr+4
   STA   __local_main__arr+5
   INY
   STY   __local_main__arr+6
   STA   __local_main__arr+7
   LDA   #$02
   STA   __zpabi_subscript_char__idx
   LDX   #>__local_main__arr
   LDA   #<__local_main__arr
   JSR   subscript_char
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@7
   LDA   __local_main__1
   CMP   #$0D
   BEQ   .if_end@8
.jcmp_skip@7:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@8:
   LDA   #>__local_main__arr
   STA   __local_main__2
   LDA   #<__local_main__arr
   CLC
   ADC   #$02
   STA   __local_main__0
   BCC   .carry_inc@0
   INC   __local_main__2
.carry_inc@0:
   LDA   #$01
   STA   __zpabi_sub_char_from_pointer__idx
   LDX   __local_main__2
   LDA   __local_main__0
   JSR   sub_char_from_pointer
   STA   __local_main__3
   TXA
   CMP   #>__local_main__arr
   BNE   .jcmp_skip@8
   LDA   __local_main__3
   CMP   #<__local_main__arr
   BEQ   .if_end@9
.jcmp_skip@8:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@9:
   LDA   #$0C
   STA   __zpabi_and_char__i_0
   LDA   #$00
   STA   __zpabi_and_char__i_1
   JSR   and_char
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$08
   RTS
.if_end@11:
   LDX   #$00
   TXA
   JSR   or_char
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$09
   RTS
.if_end@12:
   JSR   test_for_loop_char
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@13
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@13:
   LDX   #$00
   TXA
   RTS
