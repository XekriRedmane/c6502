SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_accept_params__c_doubled_0	EQU	$80
__zpabi_accept_params__c_doubled_1	EQU	$81
__zpabi_accept_params__l_cast_0	EQU	$82
__zpabi_accept_params__l_cast_1	EQU	$83
__zpabi_accept_params__l_cast_2	EQU	$84
__zpabi_accept_params__l_cast_3	EQU	$85
__zpabi_accept_params__l_cast_4	EQU	$86
__zpabi_accept_params__l_cast_5	EQU	$87
__zpabi_accept_params__l_cast_6	EQU	$88
__zpabi_accept_params__l_cast_7	EQU	$89
__zpabi_accept_params__dereferenced_ptr_0	EQU	$8A
__zpabi_accept_params__dereferenced_ptr_1	EQU	$8B
__zpabi_accept_params__d_0	EQU	$8C
__zpabi_accept_params__d_1	EQU	$8D
__zpabi_accept_params__d_2	EQU	$8E
__zpabi_accept_params__d_3	EQU	$8F
__zpabi_accept_params__d_4	EQU	$90
__zpabi_accept_params__d_5	EQU	$91
__zpabi_accept_params__d_6	EQU	$92
__zpabi_accept_params__d_7	EQU	$93
__zpabi_accept_params__c_0	EQU	$94
__zpabi_accept_params__c_1	EQU	$95
__zpabi_accept_params__l_0	EQU	$96
__zpabi_accept_params__l_1	EQU	$97
__zpabi_accept_params__l_2	EQU	$98
__zpabi_accept_params__l_3	EQU	$99
__zpabi_accept_params__ptr_0	EQU	$9A
__zpabi_accept_params__ptr_1	EQU	$9B
__local_main__0	EQU	$9C
__local_test_auto__0	EQU	$9E
__local_test_static__0	EQU	$9E
__local_test_auto__1	EQU	$9F
__local_test_static__1	EQU	$9F
__local_test_auto__2	EQU	$A0
__local_test_auto__3	EQU	$A1
__local_test_auto__autom_13	EQU	$A2
__local_test_auto__autom_14	EQU	$A3
__local_test_auto__autom_12	EQU	$A4
__local_test_auto__autom_11	EQU	$A5
__local_test_auto__autom_10	EQU	$A6
__local_test_auto__autom_9	EQU	$A7
__local_test_auto__autom_8	EQU	$A8
__local_test_auto__autom_7	EQU	$A9
__local_test_auto__autom_6	EQU	$AA
__local_test_auto__autom_5	EQU	$AB
__local_test_static__13	EQU	$AB
__local_test_auto__autom_4	EQU	$AC
__local_test_static__14	EQU	$AC
__local_test_auto__autom_3	EQU	$AD
__local_test_static__15	EQU	$AD
__local_test_auto__autom_2	EQU	$AE
__local_test_static__16	EQU	$AE
__local_test_auto__autom_1	EQU	$AF
__local_test_static__17	EQU	$AF
__local_test_auto__autom_0	EQU	$B0
__local_test_static__18	EQU	$B0
__local_test_auto__chr	EQU	$B1
__local_accept_params__d_divided_0	EQU	$B6

; @zp-link-meta-begin
; def accept_params params=__zpabi_accept_params__d_divided_0,__zpabi_accept_params__d_divided_1,__zpabi_accept_params__c_doubled_0,__zpabi_accept_params__c_doubled_1,__zpabi_accept_params__l_cast_0,__zpabi_accept_params__l_cast_1,__zpabi_accept_params__l_cast_2,__zpabi_accept_params__l_cast_3,__zpabi_accept_params__l_cast_4,__zpabi_accept_params__l_cast_5,__zpabi_accept_params__l_cast_6,__zpabi_accept_params__l_cast_7,__zpabi_accept_params__dereferenced_ptr_0,__zpabi_accept_params__dereferenced_ptr_1,__zpabi_accept_params__d_0,__zpabi_accept_params__d_1,__zpabi_accept_params__d_2,__zpabi_accept_params__d_3,__zpabi_accept_params__d_4,__zpabi_accept_params__d_5,__zpabi_accept_params__d_6,__zpabi_accept_params__d_7,__zpabi_accept_params__c_0,__zpabi_accept_params__c_1,__zpabi_accept_params__l_0,__zpabi_accept_params__l_1,__zpabi_accept_params__l_2,__zpabi_accept_params__l_3,__zpabi_accept_params__ptr_0,__zpabi_accept_params__ptr_1 locals=__local_accept_params__0,__local_accept_params__d_divided_1,__local_accept_params__d_divided_0,__local_accept_params__1,__local_accept_params__2,__local_accept_params__3,__local_accept_params__4,__local_accept_params__5 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,,,,,,,,,,,,,,
; def get_double params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_auto params= locals=__local_test_auto__0,__local_test_auto__1,__local_test_auto__2,__local_test_auto__3,__local_test_auto__autom_13,__local_test_auto__autom_14,__local_test_auto__autom_12,__local_test_auto__autom_11,__local_test_auto__autom_10,__local_test_auto__autom_9,__local_test_auto__autom_8,__local_test_auto__autom_7,__local_test_auto__autom_6,__local_test_auto__autom_5,__local_test_auto__autom_4,__local_test_auto__autom_3,__local_test_auto__autom_2,__local_test_auto__autom_1,__local_test_auto__autom_0,__local_test_auto__chr,__local_test_auto__5,__local_test_auto__6 indirect=false in_cycle=false
; def test_static params= locals=__local_test_static__0,__local_test_static__1,__local_test_static__2,__local_test_static__3,__local_test_static__4,__local_test_static__5,__local_test_static__6,__local_test_static__7,__local_test_static__8,__local_test_static__9,__local_test_static__10,__local_test_static__11,__local_test_static__12,__local_test_static__13,__local_test_static__14,__local_test_static__15,__local_test_static__16,__local_test_static__17,__local_test_static__18 indirect=false in_cycle=false
; call main -> test_auto
; call main -> test_static
; call test_auto -> accept_params
; call test_auto -> get_double
; call test_static -> accept_params
; call test_static -> get_double
; item get_double section=.text local=false zero=false
; item accept_params section=.text local=false zero=false
; item accept_params@tail2 section=.text local=true zero=false
; item accept_params@tail1 section=.text local=true zero=false
; item test_auto section=.text local=false zero=false
; item test_auto@run5 section=.text local=true zero=false
; item test_auto@run5@run7 section=.text local=true zero=false
; item test_auto@run4 section=.text local=true zero=false
; item test_auto@run3 section=.text local=true zero=false
; item test_auto@run1 section=.text local=true zero=false
; item test_auto@run0 section=.text local=true zero=false
; item test_auto@tail0 section=.text local=true zero=false
; item test_static section=.text local=false zero=false
; item test_static@run6 section=.text local=true zero=false
; item test_static@run2 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item l section=.data local=true zero=false
; item @12.stat section=.bss local=true zero=true
; item @13.chr section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

get_double:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STA   HARGS+18
   LDA   #$A2
   STA   HARGS+19
   LDA   #$94
   STA   HARGS+20
   LDA   #$1A
   STA   HARGS+21
   LDA   #$7D
   STA   HARGS+22
   LDA   #$42
   STA   HARGS+23
   RTS

accept_params:
   SUBROUTINE

   STA   __local_accept_params__d_divided_0
   LDA   __zpabi_accept_params__d_7
   CMP   #$42
   BEQ   .lb_skip@12
   JMP   .accept_params@jthread@6
.lb_skip@12:
   LDA   __zpabi_accept_params__d_6
   CMP   #$8D
   BEQ   .lb_skip@11
   JMP   .accept_params@jthread@6
.lb_skip@11:
   LDA   __zpabi_accept_params__d_5
   CMP   #$1A
   BEQ   .lb_skip@10
   JMP   .accept_params@jthread@6
.lb_skip@10:
   LDA   __zpabi_accept_params__d_4
   CMP   #$94
   BEQ   .lb_skip@9
   JMP   .accept_params@jthread@6
.lb_skip@9:
   LDA   __zpabi_accept_params__d_3
   CMP   #$A2
   BEQ   .lb_skip@8
   JMP   .accept_params@jthread@6
.lb_skip@8:
   LDA   __zpabi_accept_params__d_2
   BEQ   .lb_skip@7
   JMP   .accept_params@jthread@6
.lb_skip@7:
   LDA   __zpabi_accept_params__d_1
   BEQ   .lb_skip@6
   JMP   .accept_params@jthread@6
.lb_skip@6:
   LDA   __zpabi_accept_params__d_0
   BEQ   .lb_skip@5
   JMP   .accept_params@jthread@6
.lb_skip@5:
   LDA   __zpabi_accept_params__c_1
   BEQ   .lb_skip@4
   JMP   .accept_params@jthread@6
.lb_skip@4:
   LDA   __zpabi_accept_params__c_0
   CMP   #$7F
   BEQ   .lb_skip@3
   JMP   .accept_params@jthread@6
.lb_skip@3:
   LDA   __zpabi_accept_params__l_3
   BEQ   .lb_skip@2
   JMP   .accept_params@jthread@6
.lb_skip@2:
   LDA   __zpabi_accept_params__l_2
   CMP   #$01
   BEQ   .lb_skip@1
   JMP   .accept_params@jthread@6
.lb_skip@1:
   LDA   __zpabi_accept_params__l_1
   CMP   #$E2
   BEQ   .lb_skip@0
   JMP   .accept_params@jthread@6
.lb_skip@0:
   LDA   __zpabi_accept_params__l_0
   CMP   #$40
   BNE   .accept_params@jthread@6
   LDY   #$00
   LDA   (__zpabi_accept_params__ptr_0),Y
   CMP   #$64
   BNE   .accept_params@jthread@6
   LDA   __local_accept_params__d_divided_0
   STA   HARGS
   STX   HARGS+1
   JSR   i2d
   LDA   HARGS+9
   CMP   #$40
   BNE   .accept_params@jthread@6
   LDA   HARGS+8
   CMP   #$59
   BNE   .accept_params@jthread@6
   LDA   HARGS+7
   BNE   .accept_params@jthread@6
   LDA   HARGS+6
   BNE   .accept_params@jthread@6
   LDA   HARGS+5
   BNE   .accept_params@jthread@6
   LDA   HARGS+4
   BNE   .accept_params@jthread@6
   LDA   HARGS+3
   BNE   .accept_params@jthread@6
   LDA   HARGS+2
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__c_doubled_1
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__c_doubled_0
   CMP   #$FE
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__l_cast_7
   CMP   #$40
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__l_cast_6
   CMP   #$FE
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__l_cast_5
   CMP   #$24
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__l_cast_4
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__l_cast_3
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__l_cast_2
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__l_cast_1
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__l_cast_0
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__dereferenced_ptr_1
   BNE   .accept_params@jthread@6
   LDA   __zpabi_accept_params__dereferenced_ptr_0
   CMP   #$64
   BEQ   .if_end@14
.accept_params@jthread@6:
   JMP   accept_params@tail1
.if_end@14:
   JMP   accept_params@tail2

accept_params@tail2:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

accept_params@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_auto:
   SUBROUTINE

   JSR   test_auto@run5
   STA   __local_test_auto__autom_0
   LDA   HARGS+17
   STA   __local_test_auto__autom_1
   LDA   HARGS+18
   STA   __local_test_auto__autom_2
   LDA   HARGS+19
   STA   __local_test_auto__autom_3
   LDA   HARGS+20
   STA   __local_test_auto__autom_4
   LDA   HARGS+21
   STA   __local_test_auto__autom_5
   LDA   HARGS+22
   STA   __local_test_auto__autom_6
   LDA   HARGS+23
   STA   __local_test_auto__autom_7
   LDA   #$7F
   STA   __local_test_auto__autom_8
   JSR   test_auto@run3
   STA   __local_test_auto__autom_9
   LDA   HARGS+9
   STA   __local_test_auto__autom_10
   LDA   HARGS+10
   STA   __local_test_auto__autom_11
   LDA   HARGS+11
   STA   __local_test_auto__autom_12
   LDA   #$64
   STA   __local_test_auto__chr
   LDA   #<__local_test_auto__chr
   STA   __local_test_auto__autom_13
   LDA   #>__local_test_auto__chr
   STA   __local_test_auto__autom_14
   LDA   __local_test_auto__autom_7
   CMP   #$42
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_6
   CMP   #$8D
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_5
   CMP   #$1A
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_4
   CMP   #$94
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_3
   CMP   #$A2
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_2
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_1
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_0
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_8
   CMP   #$7F
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_12
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_11
   CMP   #$01
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_10
   CMP   #$E2
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__autom_9
   CMP   #$40
   BNE   .test_auto@jthread@2
   LDA   #>__local_test_auto__chr
   CMP   #>__local_test_auto__chr
   BNE   .test_auto@jthread@2
   LDA   #<__local_test_auto__chr
   CMP   #<__local_test_auto__chr
   BEQ   .if_end@25
.test_auto@jthread@2:
   JMP   accept_params@tail1
.if_end@25:
   LDA   __local_test_auto__autom_0
   STA   HARGS
   LDA   __local_test_auto__autom_1
   STA   HARGS+1
   LDA   __local_test_auto__autom_2
   STA   HARGS+2
   LDA   __local_test_auto__autom_3
   STA   HARGS+3
   LDA   __local_test_auto__autom_4
   STA   HARGS+4
   LDA   __local_test_auto__autom_5
   STA   HARGS+5
   LDA   __local_test_auto__autom_6
   STA   HARGS+6
   LDA   __local_test_auto__autom_7
   JSR   test_auto@run1
   STA   __local_test_auto__3
   LDA   HARGS+9
   STA   __local_test_auto__2
   LDA   __local_test_auto__autom_8
   ASL
   STA   __local_test_auto__1
   LDA   #$00
   ROL
   STA   __local_test_auto__0
   LDA   __local_test_auto__autom_9
   STA   HARGS
   LDA   __local_test_auto__autom_10
   STA   HARGS+1
   LDA   __local_test_auto__autom_11
   STA   HARGS+2
   LDA   __local_test_auto__autom_12
   JSR   test_auto@run4
   LDY   #$00
   LDA   (__local_test_auto__autom_13),Y
   STA   __zpabi_accept_params__dereferenced_ptr_0
   LDA   __local_test_auto__1
   STA   __zpabi_accept_params__c_doubled_0
   LDA   __local_test_auto__0
   STA   __zpabi_accept_params__c_doubled_1
   LDA   #$00
   STA   __zpabi_accept_params__dereferenced_ptr_1
   LDA   __local_test_auto__autom_0
   STA   __zpabi_accept_params__d_0
   LDA   __local_test_auto__autom_1
   STA   __zpabi_accept_params__d_1
   LDA   __local_test_auto__autom_2
   STA   __zpabi_accept_params__d_2
   LDA   __local_test_auto__autom_3
   STA   __zpabi_accept_params__d_3
   LDA   __local_test_auto__autom_4
   STA   __zpabi_accept_params__d_4
   LDA   __local_test_auto__autom_5
   STA   __zpabi_accept_params__d_5
   LDA   __local_test_auto__autom_6
   STA   __zpabi_accept_params__d_6
   LDA   __local_test_auto__autom_7
   STA   __zpabi_accept_params__d_7
   LDA   __local_test_auto__autom_8
   STA   __zpabi_accept_params__c_0
   LDA   #$00
   STA   __zpabi_accept_params__c_1
   LDA   __local_test_auto__autom_9
   STA   __zpabi_accept_params__l_0
   LDA   __local_test_auto__autom_10
   STA   __zpabi_accept_params__l_1
   LDA   __local_test_auto__autom_11
   STA   __zpabi_accept_params__l_2
   LDA   __local_test_auto__autom_12
   STA   __zpabi_accept_params__l_3
   LDA   __local_test_auto__autom_13
   STA   __zpabi_accept_params__ptr_0
   LDA   __local_test_auto__autom_14
   STA   __zpabi_accept_params__ptr_1
   LDX   __local_test_auto__2
   LDA   __local_test_auto__3
   JSR   accept_params
   STX   __local_test_auto__0
   ORA   __local_test_auto__0
   JMP   test_auto@tail0

test_auto@run5:
   SUBROUTINE

   JSR   get_double
   JSR   test_auto@run0
   JSR   test_auto@run5@run7
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+16
   RTS

test_auto@run5@run7:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   RTS

test_auto@run4:
   SUBROUTINE

   STA   HARGS+3
   JSR   l2d
   LDA   HARGS+4
   STA   __zpabi_accept_params__l_cast_0
   LDA   HARGS+5
   STA   __zpabi_accept_params__l_cast_1
   LDA   HARGS+6
   STA   __zpabi_accept_params__l_cast_2
   LDA   HARGS+7
   STA   __zpabi_accept_params__l_cast_3
   LDA   HARGS+8
   STA   __zpabi_accept_params__l_cast_4
   LDA   HARGS+9
   STA   __zpabi_accept_params__l_cast_5
   LDA   HARGS+10
   STA   __zpabi_accept_params__l_cast_6
   LDA   HARGS+11
   STA   __zpabi_accept_params__l_cast_7
   RTS

test_auto@run3:
   SUBROUTINE

   LDA   l
   STA   HARGS
   LDA   l+1
   STA   HARGS+1
   LDA   l+2
   STA   HARGS+2
   LDA   l+3
   STA   HARGS+3
   LDA   #$04
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   RTS

test_auto@run1:
   SUBROUTINE

   STA   HARGS+7
   JSR   test_auto@run5@run7
   STA   HARGS+10
   LDA   #$20
   STA   HARGS+11
   LDA   #$5F
   STA   HARGS+12
   LDA   #$A0
   STA   HARGS+13
   LDA   #$22
   STA   HARGS+14
   LDA   #$42
   STA   HARGS+15
   JSR   ddiv
   JSR   test_auto@run0
   JSR   d2i
   LDA   HARGS+8
   RTS

test_auto@run0:
   SUBROUTINE

   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   STA   HARGS+4
   LDA   HARGS+21
   STA   HARGS+5
   LDA   HARGS+22
   STA   HARGS+6
   LDA   HARGS+23
   STA   HARGS+7
   RTS

test_auto@tail0:
   SUBROUTINE

   BNE   .if_end@26
   JMP   accept_params@tail1
.if_end@26:
   JMP   accept_params@tail2

test_static:
   SUBROUTINE

   JSR   test_auto@run5
   STA   @12.stat
   LDA   HARGS+17
   STA   @12.stat+1
   LDA   HARGS+18
   STA   @12.stat+2
   LDA   HARGS+19
   STA   @12.stat+3
   LDA   HARGS+20
   STA   @12.stat+4
   LDA   HARGS+21
   STA   @12.stat+5
   LDA   HARGS+22
   STA   @12.stat+6
   LDA   HARGS+23
   STA   @12.stat+7
   LDA   #$7F
   STA   @12.stat+8
   JSR   test_auto@run3
   STA   @12.stat+9
   LDA   HARGS+9
   STA   @12.stat+10
   LDA   HARGS+10
   STA   @12.stat+11
   LDA   HARGS+11
   STA   @12.stat+12
   LDA   #<@13.chr
   STA   @12.stat+13
   LDA   #>@13.chr
   STA   @12.stat+14
   JSR   test_static@run2
   CMP   #$42
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__d_6
   CMP   #$8D
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__d_5
   CMP   #$1A
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__d_4
   CMP   #$94
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__d_3
   CMP   #$A2
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__d_2
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__d_1
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__d_0
   BNE   .test_static@jthread@2
   LDA   @12.stat+8
   STA   __zpabi_accept_params__c_0
   CMP   #$7F
   BNE   .test_static@jthread@2
   JSR   test_static@run6
   STA   __local_test_static__1
   LDA   @12.stat+12
   BNE   .test_static@jthread@2
   LDA   __local_test_static__1
   CMP   #$01
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__l_1
   CMP   #$E2
   BNE   .test_static@jthread@2
   LDA   __zpabi_accept_params__l_0
   CMP   #$40
   BNE   .test_static@jthread@2
   LDA   @12.stat+13
   STA   __local_test_static__0
   LDA   @12.stat+14
   STA   __local_test_static__1
   CMP   #>@13.chr
   BNE   .test_static@jthread@2
   LDA   __local_test_static__0
   CMP   #<@13.chr
   BEQ   .if_end@33
.test_static@jthread@2:
   JMP   accept_params@tail1
.if_end@33:
   LDA   __zpabi_accept_params__d_7
   CMP   #$42
   BNE   .test_static@jthread@3
   LDA   __zpabi_accept_params__d_6
   CMP   #$8D
   BNE   .test_static@jthread@3
   LDA   __zpabi_accept_params__d_5
   CMP   #$1A
   BNE   .test_static@jthread@3
   LDA   __zpabi_accept_params__d_4
   CMP   #$94
   BNE   .test_static@jthread@3
   LDA   __zpabi_accept_params__d_3
   CMP   #$A2
   BNE   .test_static@jthread@3
   LDA   __zpabi_accept_params__d_2
   BNE   .test_static@jthread@3
   LDA   __zpabi_accept_params__d_1
   BNE   .test_static@jthread@3
   LDA   __zpabi_accept_params__d_0
   BNE   .test_static@jthread@3
   LDA   __zpabi_accept_params__c_0
   CMP   #$7F
   BEQ   .if_end@36
.test_static@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@36:
   LDY   #$00
   LDA   (__local_test_static__0),Y
   CMP   #$64
   BEQ   .if_end@37
   LDX   #$00
   TXA
   RTS
.if_end@37:
   LDA   __zpabi_accept_params__d_0
   STA   HARGS
   LDA   __zpabi_accept_params__d_1
   STA   HARGS+1
   LDA   __zpabi_accept_params__d_2
   STA   HARGS+2
   LDA   __zpabi_accept_params__d_3
   STA   HARGS+3
   LDA   __zpabi_accept_params__d_4
   STA   HARGS+4
   LDA   __zpabi_accept_params__d_5
   STA   HARGS+5
   LDA   __zpabi_accept_params__d_6
   STA   HARGS+6
   LDA   __zpabi_accept_params__d_7
   JSR   test_auto@run1
   STA   __local_test_static__18
   LDA   HARGS+9
   STA   __local_test_static__17
   LDA   @12.stat+8
   ASL
   STA   __local_test_static__16
   LDA   #$00
   ROL
   STA   __local_test_static__15
   LDA   @12.stat+9
   STA   HARGS
   LDA   @12.stat+10
   STA   HARGS+1
   LDA   @12.stat+11
   STA   HARGS+2
   LDA   @12.stat+12
   JSR   test_auto@run4
   LDA   @12.stat+13
   STA   __local_test_static__13
   LDA   @12.stat+14
   STA   __local_test_static__14
   LDY   #$00
   LDA   (__local_test_static__13),Y
   STA   __zpabi_accept_params__dereferenced_ptr_0
   JSR   test_static@run2
   LDA   @12.stat+8
   STA   __zpabi_accept_params__c_0
   JSR   test_static@run6
   LDX   @12.stat+12
   STX   __local_test_static__0
   LDX   __local_test_static__16
   STX   __zpabi_accept_params__c_doubled_0
   LDX   __local_test_static__15
   STX   __zpabi_accept_params__c_doubled_1
   LDX   #$00
   STX   __zpabi_accept_params__dereferenced_ptr_1
   STX   __zpabi_accept_params__c_1
   STA   __zpabi_accept_params__l_2
   LDA   __local_test_static__0
   STA   __zpabi_accept_params__l_3
   LDA   __local_test_static__13
   STA   __zpabi_accept_params__ptr_0
   LDA   __local_test_static__14
   STA   __zpabi_accept_params__ptr_1
   LDX   __local_test_static__17
   LDA   __local_test_static__18
   JSR   accept_params
   STX   __local_test_static__0
   ORA   __local_test_static__0
   JMP   test_auto@tail0

test_static@run6:
   SUBROUTINE

   LDA   @12.stat+9
   STA   __zpabi_accept_params__l_0
   LDA   @12.stat+10
   STA   __zpabi_accept_params__l_1
   LDA   @12.stat+11
   RTS

test_static@run2:
   SUBROUTINE

   LDA   @12.stat
   STA   __zpabi_accept_params__d_0
   LDA   @12.stat+1
   STA   __zpabi_accept_params__d_1
   LDA   @12.stat+2
   STA   __zpabi_accept_params__d_2
   LDA   @12.stat+3
   STA   __zpabi_accept_params__d_3
   LDA   @12.stat+4
   STA   __zpabi_accept_params__d_4
   LDA   @12.stat+5
   STA   __zpabi_accept_params__d_5
   LDA   @12.stat+6
   STA   __zpabi_accept_params__d_6
   LDA   @12.stat+7
   STA   __zpabi_accept_params__d_7
   RTS

main:
   SUBROUTINE

   JSR   test_auto
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@39
   JMP   accept_params@tail2
.if_end@39:
   JSR   test_static
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@40
   LDX   #$00
   LDA   #$02
   RTS
.if_end@40:
   JMP   accept_params@tail1

l:
   DC.L  $0012D687

@12.stat:
   DS.B  15

@13.chr:
   DC.B  $64
