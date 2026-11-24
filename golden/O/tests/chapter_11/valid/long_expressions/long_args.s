SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_test_sum__a_0	EQU	$80
__zpabi_test_sum__a_1	EQU	$81
__zpabi_test_sum__a_2	EQU	$82
__zpabi_test_sum__a_3	EQU	$83
__zpabi_test_sum__a_4	EQU	$84
__zpabi_test_sum__a_5	EQU	$85
__zpabi_test_sum__a_6	EQU	$86
__zpabi_test_sum__a_7	EQU	$87
__zpabi_test_sum__b_0	EQU	$88
__zpabi_test_sum__b_1	EQU	$89
__zpabi_test_sum__b_2	EQU	$8A
__zpabi_test_sum__b_3	EQU	$8B
__zpabi_test_sum__b_4	EQU	$8C
__zpabi_test_sum__b_5	EQU	$8D
__zpabi_test_sum__b_6	EQU	$8E
__zpabi_test_sum__b_7	EQU	$8F
__zpabi_test_sum__c_0	EQU	$90
__zpabi_test_sum__c_1	EQU	$91
__zpabi_test_sum__d_0	EQU	$92
__zpabi_test_sum__d_1	EQU	$93
__zpabi_test_sum__e_0	EQU	$94
__zpabi_test_sum__e_1	EQU	$95
__zpabi_test_sum__f_0	EQU	$96
__zpabi_test_sum__f_1	EQU	$97
__zpabi_test_sum__g_0	EQU	$98
__zpabi_test_sum__g_1	EQU	$99
__zpabi_test_sum__h_0	EQU	$9A
__zpabi_test_sum__h_1	EQU	$9B
__zpabi_test_sum__i_0	EQU	$9C
__zpabi_test_sum__i_1	EQU	$9D
__zpabi_test_sum__i_2	EQU	$9E
__zpabi_test_sum__i_3	EQU	$9F
__zpabi_test_sum__i_4	EQU	$A0
__zpabi_test_sum__i_5	EQU	$A1
__zpabi_test_sum__i_6	EQU	$A2
__zpabi_test_sum__i_7	EQU	$A3
__local_test_sum__0	EQU	$A5
__local_test_sum__1	EQU	$A6
__local_test_sum__2	EQU	$A7
__local_test_sum__3	EQU	$A8
__local_test_sum__4	EQU	$A9
__local_test_sum__5	EQU	$AA
__local_test_sum__6	EQU	$AB
__local_test_sum__7	EQU	$AC

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def test_sum params=__zpabi_test_sum__a_0,__zpabi_test_sum__a_1,__zpabi_test_sum__a_2,__zpabi_test_sum__a_3,__zpabi_test_sum__a_4,__zpabi_test_sum__a_5,__zpabi_test_sum__a_6,__zpabi_test_sum__a_7,__zpabi_test_sum__b_0,__zpabi_test_sum__b_1,__zpabi_test_sum__b_2,__zpabi_test_sum__b_3,__zpabi_test_sum__b_4,__zpabi_test_sum__b_5,__zpabi_test_sum__b_6,__zpabi_test_sum__b_7,__zpabi_test_sum__c_0,__zpabi_test_sum__c_1,__zpabi_test_sum__d_0,__zpabi_test_sum__d_1,__zpabi_test_sum__e_0,__zpabi_test_sum__e_1,__zpabi_test_sum__f_0,__zpabi_test_sum__f_1,__zpabi_test_sum__g_0,__zpabi_test_sum__g_1,__zpabi_test_sum__h_0,__zpabi_test_sum__h_1,__zpabi_test_sum__i_0,__zpabi_test_sum__i_1,__zpabi_test_sum__i_2,__zpabi_test_sum__i_3,__zpabi_test_sum__i_4,__zpabi_test_sum__i_5,__zpabi_test_sum__i_6,__zpabi_test_sum__i_7 locals=__local_test_sum__0,__local_test_sum__1,__local_test_sum__2,__local_test_sum__3,__local_test_sum__4,__local_test_sum__5,__local_test_sum__6,__local_test_sum__7 indirect=false in_cycle=false
; call main -> test_sum
; item test_sum section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_sum:
   SUBROUTINE

   LDA   __zpabi_test_sum__a_0
   CLC
   ADC   __zpabi_test_sum__b_0
   STA   __local_test_sum__7
   LDA   __zpabi_test_sum__a_1
   ADC   __zpabi_test_sum__b_1
   STA   __local_test_sum__6
   LDA   __zpabi_test_sum__a_2
   ADC   __zpabi_test_sum__b_2
   STA   __local_test_sum__5
   LDA   __zpabi_test_sum__a_3
   ADC   __zpabi_test_sum__b_3
   STA   __local_test_sum__4
   LDA   __zpabi_test_sum__a_4
   ADC   __zpabi_test_sum__b_4
   STA   __local_test_sum__3
   LDA   __zpabi_test_sum__a_5
   ADC   __zpabi_test_sum__b_5
   STA   __local_test_sum__2
   LDA   __zpabi_test_sum__a_6
   ADC   __zpabi_test_sum__b_6
   STA   __local_test_sum__1
   LDA   __zpabi_test_sum__a_7
   ADC   __zpabi_test_sum__b_7
   STA   __local_test_sum__0
   LDA   __local_test_sum__7
   SEC
   SBC   #$64
   LDA   __local_test_sum__6
   SBC   #$00
   LDA   __local_test_sum__5
   SBC   #$00
   LDA   __local_test_sum__4
   SBC   #$00
   LDA   __local_test_sum__3
   SBC   #$00
   LDA   __local_test_sum__2
   SBC   #$00
   LDA   __local_test_sum__1
   SBC   #$00
   LDA   __local_test_sum__0
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __zpabi_test_sum__i_0
   SEC
   SBC   #$64
   LDA   __zpabi_test_sum__i_1
   SBC   #$00
   LDA   __zpabi_test_sum__i_2
   SBC   #$00
   LDA   __zpabi_test_sum__i_3
   SBC   #$00
   LDA   __zpabi_test_sum__i_4
   SBC   #$00
   LDA   __zpabi_test_sum__i_5
   SBC   #$00
   LDA   __zpabi_test_sum__i_6
   SBC   #$00
   LDA   __zpabi_test_sum__i_7
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$40
   STA   __zpabi_test_sum__a_0
   STA   __zpabi_test_sum__b_0
   STA   __zpabi_test_sum__i_0
   LDA   #$42
   STA   __zpabi_test_sum__a_1
   STA   __zpabi_test_sum__b_1
   STA   __zpabi_test_sum__i_1
   LDA   #$0F
   STA   __zpabi_test_sum__a_2
   STA   __zpabi_test_sum__b_2
   STA   __zpabi_test_sum__i_2
   LDA   #$00
   STA   __zpabi_test_sum__a_3
   STA   __zpabi_test_sum__a_4
   STA   __zpabi_test_sum__a_5
   STA   __zpabi_test_sum__a_6
   STA   __zpabi_test_sum__a_7
   STA   __zpabi_test_sum__b_3
   STA   __zpabi_test_sum__b_4
   STA   __zpabi_test_sum__b_5
   STA   __zpabi_test_sum__b_6
   STA   __zpabi_test_sum__b_7
   STA   __zpabi_test_sum__c_0
   STA   __zpabi_test_sum__c_1
   STA   __zpabi_test_sum__d_0
   STA   __zpabi_test_sum__d_1
   STA   __zpabi_test_sum__e_0
   STA   __zpabi_test_sum__e_1
   STA   __zpabi_test_sum__f_0
   STA   __zpabi_test_sum__f_1
   STA   __zpabi_test_sum__g_0
   STA   __zpabi_test_sum__g_1
   STA   __zpabi_test_sum__h_0
   STA   __zpabi_test_sum__h_1
   STA   __zpabi_test_sum__i_3
   STA   __zpabi_test_sum__i_4
   STA   __zpabi_test_sum__i_5
   STA   __zpabi_test_sum__i_6
   STA   __zpabi_test_sum__i_7
   JMP   test_sum
