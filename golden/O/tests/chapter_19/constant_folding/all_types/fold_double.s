SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_add params= locals= indirect=false in_cycle=false
; def target_compare_infininty params= locals= indirect=false in_cycle=false
; def target_div params= locals= indirect=false in_cycle=false
; def target_div_underflow params= locals= indirect=false in_cycle=false
; def target_eq params= locals= indirect=false in_cycle=false
; def target_ge params= locals= indirect=false in_cycle=false
; def target_gt params= locals= indirect=false in_cycle=false
; def target_infinity params= locals= indirect=false in_cycle=false
; def target_le params= locals= indirect=false in_cycle=false
; def target_lt params= locals= indirect=false in_cycle=false
; def target_mult params= locals= indirect=false in_cycle=false
; def target_neg params= locals= indirect=false in_cycle=false
; def target_negate_zero params= locals= indirect=false in_cycle=false
; def target_neq params= locals= indirect=false in_cycle=false
; def target_not params= locals= indirect=false in_cycle=false
; def target_sub params= locals= indirect=false in_cycle=false
; call main -> target_add
; call main -> target_compare_infininty
; call main -> target_div
; call main -> target_div_underflow
; call main -> target_eq
; call main -> target_ge
; call main -> target_gt
; call main -> target_infinity
; call main -> target_le
; call main -> target_lt
; call main -> target_mult
; call main -> target_neg
; call main -> target_negate_zero
; call main -> target_neq
; call main -> target_not
; call main -> target_sub
; item target_add section=.text local=false zero=false
; item target_sub section=.text local=false zero=false
; item target_sub@tail0 section=.text local=true zero=false
; item target_sub@tail0@run3 section=.text local=true zero=false
; item target_mult section=.text local=false zero=false
; item target_div section=.text local=false zero=false
; item target_div_underflow section=.text local=false zero=false
; item target_div_underflow@run4 section=.text local=true zero=false
; item target_neg section=.text local=false zero=false
; item target_not section=.text local=false zero=false
; item target_eq section=.text local=false zero=false
; item target_eq@tail1 section=.text local=true zero=false
; item target_neq section=.text local=false zero=false
; item target_gt section=.text local=false zero=false
; item target_ge section=.text local=false zero=false
; item target_lt section=.text local=false zero=false
; item target_le section=.text local=false zero=false
; item target_negate_zero section=.text local=false zero=false
; item target_negate_zero@run2 section=.text local=true zero=false
; item target_infinity section=.text local=false zero=false
; item target_compare_infininty section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target_add:
   SUBROUTINE

   LDA   #$6B
   STA   HARGS+16
   LDA   #$7B
   STA   HARGS+17
   LDA   #$53
   STA   HARGS+18
   LDA   #$AC
   STA   HARGS+19
   LDA   #$59
   STA   HARGS+20
   LDA   #$95
   STA   HARGS+21
   LDA   #$68
   STA   HARGS+22
   LDA   #$4C
   STA   HARGS+23
   RTS

target_sub:
   SUBROUTINE

   LDA   #$01
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   JMP   target_sub@tail0

target_sub@tail0:
   SUBROUTINE

   JSR   target_sub@tail0@run3
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

target_sub@tail0@run3:
   SUBROUTINE

   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   RTS

target_mult:
   SUBROUTINE

   LDA   #$34
   STA   HARGS+16
   LDA   #$33
   STA   HARGS+17
   LDY   #$33
   JSR   target_sub@tail0@run3
   STA   HARGS+21
   LDA   #$19
   STA   HARGS+22
   LDA   #$40
   STA   HARGS+23
   RTS

target_div:
   SUBROUTINE

   LDA   #$B1
   STA   HARGS+16
   LDA   #$BF
   STA   HARGS+17
   LDA   #$EC
   STA   HARGS+18
   LDA   #$9E
   STA   HARGS+19
   LDA   #$3C
   STA   HARGS+20
   LDA   #$2C
   STA   HARGS+21
   LDA   #$CC
   STA   HARGS+22
   LDA   #$3F
   STA   HARGS+23
   RTS

target_div_underflow:
   SUBROUTINE

   JSR   target_div_underflow@run4
   JMP   target_sub@tail0

target_div_underflow@run4:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   RTS

target_neg:
   SUBROUTINE

   LDA   #$82
   STA   HARGS+16
   LDA   #$B8
   STA   HARGS+17
   LDA   #$61
   STA   HARGS+18
   LDA   #$CA
   STA   HARGS+19
   LDA   #$76
   STA   HARGS+20
   LDA   #$74
   STA   HARGS+21
   LDA   #$92
   STA   HARGS+22
   LDA   #$BE
   STA   HARGS+23
   RTS

target_not:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_eq:
   SUBROUTINE

   JMP   target_eq@tail1

target_eq@tail1:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_neq:
   SUBROUTINE

   JMP   target_eq@tail1

target_gt:
   SUBROUTINE

   JMP   target_eq@tail1

target_ge:
   SUBROUTINE

   JMP   target_eq@tail1

target_lt:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_le:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_negate_zero:
   SUBROUTINE

   JSR   target_negate_zero@run2
   STY   HARGS+21
   STA   HARGS+22
   LDA   #$80
   STA   HARGS+23
   RTS

target_negate_zero@run2:
   SUBROUTINE

   JSR   target_div_underflow@run4
   JSR   target_sub@tail0@run3
   RTS

target_infinity:
   SUBROUTINE

   JSR   target_negate_zero@run2
   STA   HARGS+21
   LDA   #$F0
   STA   HARGS+22
   LDA   #$7F
   STA   HARGS+23
   RTS

target_compare_infininty:
   SUBROUTINE

   JMP   target_eq@tail1

main:
   SUBROUTINE

   JSR   target_add
   LDA   HARGS+23
   CMP   #$4C
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$68
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   CMP   #$95
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   CMP   #$59
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   CMP   #$AC
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   CMP   #$53
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   CMP   #$7B
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   CMP   #$6B
   BEQ   .if_end@0
.jcmp_skip@0:
   JMP   target_eq@tail1
.if_end@0:
   JSR   target_sub
   LDA   HARGS+23
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_mult
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   CMP   #$19
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   CMP   #$33
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   CMP   #$33
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   CMP   #$33
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   CMP   #$33
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   CMP   #$33
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   CMP   #$34
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   target_div
   LDA   HARGS+23
   CMP   #$3F
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   CMP   #$CC
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   CMP   #$2C
   BNE   .jcmp_skip@3
   LDA   HARGS+20
   CMP   #$3C
   BNE   .jcmp_skip@3
   LDA   HARGS+19
   CMP   #$9E
   BNE   .jcmp_skip@3
   LDA   HARGS+18
   CMP   #$EC
   BNE   .jcmp_skip@3
   LDA   HARGS+17
   CMP   #$BF
   BNE   .jcmp_skip@3
   LDA   HARGS+16
   CMP   #$B1
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   target_div_underflow
   JSR   main@run1
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   target_neg
   JSR   main@run0
   LDA   #$82
   STA   HARGS+8
   LDA   #$B8
   STA   HARGS+9
   LDA   #$61
   STA   HARGS+10
   LDA   #$CA
   STA   HARGS+11
   LDA   #$76
   STA   HARGS+12
   LDA   #$74
   STA   HARGS+13
   LDA   #$92
   STA   HARGS+14
   LDA   #$3E
   STA   HARGS+15
   JSR   dadd
   JSR   main@run1
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   target_not
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JSR   target_eq
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   JSR   target_neq
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   JSR   target_gt
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@9
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   JSR   target_ge
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@10
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   JSR   target_lt
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   JSR   target_le
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@12:
   JSR   target_infinity
   LDA   HARGS+23
   CMP   #$7F
   BNE   .jcmp_skip@4
   LDA   HARGS+22
   CMP   #$F0
   BNE   .jcmp_skip@4
   LDA   HARGS+21
   BNE   .jcmp_skip@4
   LDA   HARGS+20
   BNE   .jcmp_skip@4
   LDA   HARGS+19
   BNE   .jcmp_skip@4
   LDA   HARGS+18
   BNE   .jcmp_skip@4
   LDA   HARGS+17
   BNE   .jcmp_skip@4
   LDA   HARGS+16
   BEQ   .if_end@13
.jcmp_skip@4:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@13:
   JSR   target_compare_infininty
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@14
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@14:
   JSR   target_negate_zero
   LDA   HARGS+16
   STA   HARGS+8
   LDA   HARGS+17
   STA   HARGS+9
   LDA   HARGS+18
   STA   HARGS+10
   LDA   HARGS+19
   STA   HARGS+11
   LDA   HARGS+20
   STA   HARGS+12
   LDA   HARGS+21
   STA   HARGS+13
   LDA   HARGS+22
   STA   HARGS+14
   LDA   HARGS+23
   STA   HARGS+15
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$F0
   STA   HARGS+6
   LDA   #$3F
   STA   HARGS+7
   JSR   ddiv
   JSR   main@run0
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   BNE   .if_end@15
   LDX   #$00
   LDA   #$10
   RTS
.if_end@15:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   ORA   HARGS+22
   ORA   HARGS+23
   RTS

main@run0:
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
