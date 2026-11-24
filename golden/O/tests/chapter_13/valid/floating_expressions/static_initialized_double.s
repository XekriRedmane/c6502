SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__8	EQU	$88
__local_main__9	EQU	$89
__local_main__10	EQU	$8A
__local_main__11	EQU	$8B
__local_main__12	EQU	$8C
__local_main__13	EQU	$8D
__local_main__14	EQU	$8E
__local_main__15	EQU	$8F
__local_main__16	EQU	$90
__local_main__17	EQU	$91
__local_main__18	EQU	$92
__local_main__19	EQU	$93
__local_main__20	EQU	$94
__local_main__21	EQU	$95
__local_main__22	EQU	$96
__local_main__23	EQU	$97
__local_return_static_variable__ret_7	EQU	$98
__local_return_static_variable__ret_6	EQU	$99
__local_return_static_variable__ret_5	EQU	$9A
__local_return_static_variable__ret_4	EQU	$9B
__local_return_static_variable__ret_3	EQU	$9C
__local_return_static_variable__ret_2	EQU	$9D
__local_return_static_variable__ret_1	EQU	$9E
__local_return_static_variable__ret_0	EQU	$9F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23 indirect=false in_cycle=false
; def return_static_variable params= locals=__local_return_static_variable__ret_7,__local_return_static_variable__ret_6,__local_return_static_variable__ret_5,__local_return_static_variable__ret_4,__local_return_static_variable__ret_3,__local_return_static_variable__ret_2,__local_return_static_variable__ret_1,__local_return_static_variable__ret_0 indirect=false in_cycle=false
; call main -> return_static_variable
; item return_static_variable section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.d section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

return_static_variable:
   SUBROUTINE

   LDA   @0.d
   STA   __local_return_static_variable__ret_0
   STA   HARGS
   LDA   @0.d+1
   STA   __local_return_static_variable__ret_1
   STA   HARGS+1
   LDA   @0.d+2
   STA   __local_return_static_variable__ret_2
   STA   HARGS+2
   LDA   @0.d+3
   STA   __local_return_static_variable__ret_3
   STA   HARGS+3
   LDA   @0.d+4
   STA   __local_return_static_variable__ret_4
   STA   HARGS+4
   LDA   @0.d+5
   STA   __local_return_static_variable__ret_5
   STA   HARGS+5
   LDA   @0.d+6
   STA   __local_return_static_variable__ret_6
   STA   HARGS+6
   LDA   @0.d+7
   STA   __local_return_static_variable__ret_7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   STA   @0.d
   LDA   HARGS+17
   STA   @0.d+1
   LDA   HARGS+18
   STA   @0.d+2
   LDA   HARGS+19
   STA   @0.d+3
   LDA   HARGS+20
   STA   @0.d+4
   LDA   HARGS+21
   STA   @0.d+5
   LDA   HARGS+22
   STA   @0.d+6
   LDA   HARGS+23
   STA   @0.d+7
   LDA   __local_return_static_variable__ret_0
   STA   HARGS+16
   LDA   __local_return_static_variable__ret_1
   STA   HARGS+17
   LDA   __local_return_static_variable__ret_2
   STA   HARGS+18
   LDA   __local_return_static_variable__ret_3
   STA   HARGS+19
   LDA   __local_return_static_variable__ret_4
   STA   HARGS+20
   LDA   __local_return_static_variable__ret_5
   STA   HARGS+21
   LDA   __local_return_static_variable__ret_6
   STA   HARGS+22
   LDA   __local_return_static_variable__ret_7
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   return_static_variable
   LDA   HARGS+16
   STA   __local_main__23
   LDA   HARGS+17
   STA   __local_main__22
   LDA   HARGS+18
   STA   __local_main__21
   LDA   HARGS+19
   STA   __local_main__20
   LDA   HARGS+20
   STA   __local_main__19
   LDA   HARGS+21
   STA   __local_main__18
   LDA   HARGS+22
   STA   __local_main__17
   LDA   HARGS+23
   STA   __local_main__16
   JSR   return_static_variable
   LDA   HARGS+16
   STA   __local_main__15
   LDA   HARGS+17
   STA   __local_main__14
   LDA   HARGS+18
   STA   __local_main__13
   LDA   HARGS+19
   STA   __local_main__12
   LDA   HARGS+20
   STA   __local_main__11
   LDA   HARGS+21
   STA   __local_main__10
   LDA   HARGS+22
   STA   __local_main__9
   LDA   HARGS+23
   STA   __local_main__8
   JSR   return_static_variable
   LDA   __local_main__16
   CMP   #$3F
   BNE   .jcmp_skip@0
   LDA   __local_main__17
   CMP   #$E0
   BNE   .jcmp_skip@0
   LDA   __local_main__18
   BNE   .jcmp_skip@0
   LDA   __local_main__19
   BNE   .jcmp_skip@0
   LDA   __local_main__20
   BNE   .jcmp_skip@0
   LDA   __local_main__21
   BNE   .jcmp_skip@0
   LDA   __local_main__22
   BNE   .jcmp_skip@0
   LDA   __local_main__23
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__8
   CMP   #$3F
   BNE   .jcmp_skip@1
   LDA   __local_main__9
   CMP   #$F8
   BNE   .jcmp_skip@1
   LDA   __local_main__10
   BNE   .jcmp_skip@1
   LDA   __local_main__11
   BNE   .jcmp_skip@1
   LDA   __local_main__12
   BNE   .jcmp_skip@1
   LDA   __local_main__13
   BNE   .jcmp_skip@1
   LDA   __local_main__14
   BNE   .jcmp_skip@1
   LDA   __local_main__15
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   CMP   #$04
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

@0.d:
   DC.L  $00000000
   DC.L  $3FE00000
