SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_test_struct_param__p_0	EQU	$80
__zpabi_test_struct_param__p_1	EQU	$81
__zpabi_test_struct_param__p_2	EQU	$82
__zpabi_test_struct_param__p_3	EQU	$83
__zpabi_test_struct_param__p_4	EQU	$84
__zpabi_test_struct_param__p_5	EQU	$85
__zpabi_test_struct_param__p_6	EQU	$86
__zpabi_test_struct_param__p_7	EQU	$87
__zpabi_test_struct_param__p_8	EQU	$88
__zpabi_test_struct_param__p_9	EQU	$89
__local_test_struct_param__1	EQU	$93
__local_test_struct_param__2	EQU	$94
__local_test_struct_param__3	EQU	$95
__local_test_struct_param__4	EQU	$96
__local_test_struct_param__5	EQU	$97
__local_test_struct_param__6	EQU	$98
__local_test_struct_param__7	EQU	$99

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def test_struct_param params=__zpabi_test_struct_param__p_0,__zpabi_test_struct_param__p_1,__zpabi_test_struct_param__p_2,__zpabi_test_struct_param__p_3,__zpabi_test_struct_param__p_4,__zpabi_test_struct_param__p_5,__zpabi_test_struct_param__p_6,__zpabi_test_struct_param__p_7,__zpabi_test_struct_param__p_8,__zpabi_test_struct_param__p_9 locals=__local_test_struct_param__0,__local_test_struct_param__1,__local_test_struct_param__2,__local_test_struct_param__3,__local_test_struct_param__4,__local_test_struct_param__5,__local_test_struct_param__6,__local_test_struct_param__7,__local_test_struct_param__8,__local_test_struct_param__9 indirect=false in_cycle=false changes=__zpabi_test_struct_param__p_0,__zpabi_test_struct_param__p_1,__zpabi_test_struct_param__p_2,__zpabi_test_struct_param__p_3,__zpabi_test_struct_param__p_4,__zpabi_test_struct_param__p_5,__zpabi_test_struct_param__p_6,__zpabi_test_struct_param__p_7,__zpabi_test_struct_param__p_8,__zpabi_test_struct_param__p_9
; call main -> test_struct_param
; item test_struct_param section=.text local=false zero=false
; item test_struct_param@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

test_struct_param:
   SUBROUTINE

   LDA   __zpabi_test_struct_param__p_0
   STA   __local_test_struct_param__1
   LDA   __zpabi_test_struct_param__p_0+1
   BNE   .test_struct_param@jthread@0
   LDA   __local_test_struct_param__1
   CMP   #$01
   BNE   .test_struct_param@jthread@0
   LDA   __zpabi_test_struct_param__p_0+2
   STA   __local_test_struct_param__7
   LDA   __zpabi_test_struct_param__p_0+3
   STA   __local_test_struct_param__6
   LDA   __zpabi_test_struct_param__p_0+4
   STA   __local_test_struct_param__5
   LDA   __zpabi_test_struct_param__p_0+5
   STA   __local_test_struct_param__4
   LDA   __zpabi_test_struct_param__p_0+6
   STA   __local_test_struct_param__3
   LDA   __zpabi_test_struct_param__p_0+7
   STA   __local_test_struct_param__2
   LDA   __zpabi_test_struct_param__p_0+8
   STA   __local_test_struct_param__1
   LDA   __zpabi_test_struct_param__p_0+9
   CMP   #$40
   BNE   .test_struct_param@jthread@0
   LDA   __local_test_struct_param__1
   BNE   .test_struct_param@jthread@0
   LDA   __local_test_struct_param__2
   BNE   .test_struct_param@jthread@0
   LDA   __local_test_struct_param__3
   BNE   .test_struct_param@jthread@0
   LDA   __local_test_struct_param__4
   BNE   .test_struct_param@jthread@0
   LDA   __local_test_struct_param__5
   BNE   .test_struct_param@jthread@0
   LDA   __local_test_struct_param__6
   BNE   .test_struct_param@jthread@0
   LDA   __local_test_struct_param__7
   BEQ   .if_end@2
.test_struct_param@jthread@0:
   JSR   test_struct_param@run0
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS
.if_end@2:
   JSR   test_struct_param@run0
   STA   HARGS+21
   LDA   #$F0
   STA   HARGS+22
   LDA   #$3F
   STA   HARGS+23
   RTS

test_struct_param@run0:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_test_struct_param__p_0
   LDA   #$00
   STA   __zpabi_test_struct_param__p_1
   LDY   #$00
   STY   __zpabi_test_struct_param__p_2
   STY   __zpabi_test_struct_param__p_3
   STY   __zpabi_test_struct_param__p_4
   STY   __zpabi_test_struct_param__p_5
   STY   __zpabi_test_struct_param__p_6
   STY   __zpabi_test_struct_param__p_7
   STA   __zpabi_test_struct_param__p_8
   LDA   #$40
   STA   __zpabi_test_struct_param__p_9
   JSR   test_struct_param
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   ORA   HARGS+22
   ORA   HARGS+23
   BNE   .if_end@3
   LDX   #$00
   LDA   #$01
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
