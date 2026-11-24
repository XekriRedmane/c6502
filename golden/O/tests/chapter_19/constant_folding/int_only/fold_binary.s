SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87
__local_target_ge_false__0	EQU	$90
__local_target_lt_true__0	EQU	$90

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15 indirect=false in_cycle=false
; def target_add params= locals= indirect=false in_cycle=false
; def target_div params= locals= indirect=false in_cycle=false
; def target_eq_false params= locals= indirect=false in_cycle=false
; def target_eq_true params= locals= indirect=false in_cycle=false
; def target_ge_false params= locals=__local_target_ge_false__0 indirect=false in_cycle=false
; def target_ge_true params= locals= indirect=false in_cycle=false
; def target_gt_false params= locals= indirect=false in_cycle=false
; def target_gt_true params= locals= indirect=false in_cycle=false
; def target_le_false params= locals= indirect=false in_cycle=false
; def target_le_true params= locals= indirect=false in_cycle=false
; def target_lt_false params= locals= indirect=false in_cycle=false
; def target_lt_true params= locals=__local_target_lt_true__0 indirect=false in_cycle=false
; def target_mult params= locals= indirect=false in_cycle=false
; def target_neq_false params= locals= indirect=false in_cycle=false
; def target_neq_true params= locals= indirect=false in_cycle=false
; def target_rem params= locals= indirect=false in_cycle=false
; def target_sub params= locals= indirect=false in_cycle=false
; call main -> target_add
; call main -> target_div
; call main -> target_eq_false
; call main -> target_eq_true
; call main -> target_ge_false
; call main -> target_ge_true
; call main -> target_gt_false
; call main -> target_gt_true
; call main -> target_le_false
; call main -> target_le_true
; call main -> target_lt_false
; call main -> target_lt_true
; call main -> target_mult
; call main -> target_neq_false
; call main -> target_neq_true
; call main -> target_rem
; call main -> target_sub
; item target_add section=.text local=false zero=false
; item target_add@tail3 section=.text local=true zero=false
; item target_add@tail2 section=.text local=true zero=false
; item target_add@tail2@tail6 section=.text local=true zero=false
; item target_add@tail2@tail4 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18@tail19 section=.text local=true zero=false
; item target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18@tail19@tail20 section=.text local=true zero=false
; item target_sub section=.text local=false zero=false
; item target_sub@run4 section=.text local=true zero=false
; item target_mult section=.text local=false zero=false
; item target_div section=.text local=false zero=false
; item target_rem section=.text local=false zero=false
; item target_rem@tail0 section=.text local=true zero=false
; item target_rem@tail0@tail5 section=.text local=true zero=false
; item target_rem@tail0@tail5@run2 section=.text local=true zero=false
; item target_eq_true section=.text local=false zero=false
; item target_eq_false section=.text local=false zero=false
; item target_eq_false@tail1 section=.text local=true zero=false
; item target_neq_true section=.text local=false zero=false
; item target_neq_false section=.text local=false zero=false
; item target_gt_true section=.text local=false zero=false
; item target_gt_false section=.text local=false zero=false
; item target_ge_true section=.text local=false zero=false
; item target_ge_false section=.text local=false zero=false
; item target_ge_false@run3 section=.text local=true zero=false
; item target_ge_false@tail21 section=.text local=true zero=false
; item target_lt_true section=.text local=false zero=false
; item target_lt_false section=.text local=false zero=false
; item target_le_true section=.text local=false zero=false
; item target_le_false section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item val_to_negate section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target_add:
   SUBROUTINE

   LDA   #$2C
   JMP   target_add@tail3

target_add@tail3:
   SUBROUTINE

   STA   HARGS+16
   LDA   #$01
   STA   HARGS+17
   LDA   #$00
   STA   HARGS+18
   LDY   #$00
   JMP   target_add@tail2

target_add@tail2:
   SUBROUTINE

   STY   HARGS+19
   JMP   target_add@tail2@tail6

target_add@tail2@tail6:
   SUBROUTINE

   STY   HARGS+20
   JMP   target_add@tail2@tail4

target_add@tail2@tail4:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7

target_add@tail2@tail4@tail7:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8

target_add@tail2@tail4@tail7@tail8:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9

target_add@tail2@tail4@tail7@tail8@tail9:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10

target_add@tail2@tail4@tail7@tail8@tail9@tail10:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18@tail19

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18@tail19:
   SUBROUTINE

   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18@tail19@tail20

target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18@tail19@tail20:
   SUBROUTINE

   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

target_sub:
   SUBROUTINE

   LDA   #$03
   JSR   target_sub@run4
   STA   HARGS+18
   LDA   #$80
   STA   HARGS+19
   LDA   #$FF
   STA   HARGS+20
   LDY   #$FF
   JMP   target_add@tail2@tail4

target_sub@run4:
   SUBROUTINE

   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS

target_mult:
   SUBROUTINE

   LDA   #$40
   STA   HARGS+16
   LDA   #$42
   STA   HARGS+17
   LDA   #$0F
   STA   HARGS+18
   LDA   #$00
   STA   HARGS+19
   LDY   #$00
   JMP   target_add@tail2@tail6

target_div:
   SUBROUTINE

   LDA   #$15
   JMP   target_add@tail3

target_rem:
   SUBROUTINE

   JMP   target_rem@tail0

target_rem@tail0:
   SUBROUTINE

   LDA   #$01
   JMP   target_rem@tail0@tail5

target_rem@tail0@tail5:
   SUBROUTINE

   JSR   target_rem@tail0@tail5@run2
   JMP   target_add@tail2

target_rem@tail0@tail5@run2:
   SUBROUTINE

   JSR   target_sub@run4
   LDY   #$00
   STY   HARGS+18
   RTS

target_eq_true:
   SUBROUTINE

   JMP   target_rem@tail0

target_eq_false:
   SUBROUTINE

   JMP   target_eq_false@tail1

target_eq_false@tail1:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   JMP   target_add@tail2

target_neq_true:
   SUBROUTINE

   JMP   target_rem@tail0

target_neq_false:
   SUBROUTINE

   JMP   target_eq_false@tail1

target_gt_true:
   SUBROUTINE

   JMP   target_rem@tail0

target_gt_false:
   SUBROUTINE

   JMP   target_eq_false@tail1

target_ge_true:
   SUBROUTINE

   JMP   target_rem@tail0

target_ge_false:
   SUBROUTINE

   LDA   #$63
   SEC
   SBC   #$40
   LDA   #$08
   SBC   #$E2
   LDA   #$00
   SBC   #$01
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_target_ge_false__0
   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   JSR   target_ge_false@run3
   LDA   __local_target_ge_false__0
   JMP   target_ge_false@tail21

target_ge_false@run3:
   SUBROUTINE

   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   RTS

target_ge_false@tail21:
   SUBROUTINE

   JSR   target_sub@run4
   RTS

target_lt_true:
   SUBROUTINE

   LDA   #$00
   CLC
   SBC   #$00
   LDA   #$01
   SBC   #$01
   LDA   #$00
   SBC   #$00
   LDA   #$40
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_target_lt_true__0
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   JSR   target_ge_false@run3
   LDA   __local_target_lt_true__0
   JMP   target_ge_false@tail21

target_lt_false:
   SUBROUTINE

   JMP   target_eq_false@tail1

target_le_true:
   SUBROUTINE

   JMP   target_rem@tail0

target_le_false:
   SUBROUTINE

   JMP   target_eq_false@tail1

main:
   SUBROUTINE

   JSR   target_add
   LDA   HARGS+23
   BNE   .jcmp_skip@4
   LDA   HARGS+22
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
   CMP   #$01
   BNE   .jcmp_skip@4
   LDA   HARGS+16
   CMP   #$2C
   BEQ   .if_end@0
.jcmp_skip@4:
   JMP   target_rem@tail0
.if_end@0:
   JSR   target_sub
   SEC
   LDA   #$00
   SBC   val_to_negate
   STA   __local_main__7
   LDA   #$00
   SBC   val_to_negate+1
   STA   __local_main__6
   LDA   #$00
   SBC   val_to_negate+2
   STA   __local_main__5
   LDA   #$00
   SBC   val_to_negate+3
   STA   __local_main__4
   LDA   #$00
   SBC   val_to_negate+4
   STA   __local_main__3
   LDA   #$00
   SBC   val_to_negate+5
   STA   __local_main__2
   LDA   #$00
   SBC   val_to_negate+6
   STA   __local_main__1
   LDA   #$00
   SBC   val_to_negate+7
   STA   __local_main__0
   LDA   HARGS+23
   CMP   __local_main__0
   BNE   .jcmp_skip@5
   LDA   HARGS+22
   CMP   __local_main__1
   BNE   .jcmp_skip@5
   LDA   HARGS+21
   CMP   __local_main__2
   BNE   .jcmp_skip@5
   LDA   HARGS+20
   CMP   __local_main__3
   BNE   .jcmp_skip@5
   LDA   HARGS+19
   CMP   __local_main__4
   BNE   .jcmp_skip@5
   LDA   HARGS+18
   CMP   __local_main__5
   BNE   .jcmp_skip@5
   LDA   HARGS+17
   CMP   __local_main__6
   BNE   .jcmp_skip@5
   LDA   HARGS+16
   CMP   __local_main__7
   BEQ   .if_end@1
.jcmp_skip@5:
   LDA   #$02
   JMP   target_rem@tail0@tail5
.if_end@1:
   JSR   target_mult
   LDA   HARGS+23
   BNE   .jcmp_skip@6
   LDA   HARGS+22
   BNE   .jcmp_skip@6
   LDA   HARGS+21
   BNE   .jcmp_skip@6
   LDA   HARGS+20
   BNE   .jcmp_skip@6
   LDA   HARGS+19
   BNE   .jcmp_skip@6
   LDA   HARGS+18
   CMP   #$0F
   BNE   .jcmp_skip@6
   LDA   HARGS+17
   CMP   #$42
   BNE   .jcmp_skip@6
   LDA   HARGS+16
   CMP   #$40
   BEQ   .if_end@2
.jcmp_skip@6:
   LDA   #$03
   JSR   target_rem@tail0@tail5@run2
   STY   HARGS+19
   JMP   target_add@tail2@tail6
.if_end@2:
   JSR   target_div
   LDA   HARGS+23
   BNE   .jcmp_skip@7
   LDA   HARGS+22
   BNE   .jcmp_skip@7
   LDA   HARGS+21
   BNE   .jcmp_skip@7
   LDA   HARGS+20
   BNE   .jcmp_skip@7
   LDA   HARGS+19
   BNE   .jcmp_skip@7
   LDA   HARGS+18
   BNE   .jcmp_skip@7
   LDA   HARGS+17
   CMP   #$01
   BNE   .jcmp_skip@7
   LDA   HARGS+16
   CMP   #$15
   BEQ   .if_end@3
.jcmp_skip@7:
   LDA   #$04
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7
.if_end@3:
   JSR   target_rem
   LDA   HARGS+23
   BNE   .jcmp_skip@8
   LDA   HARGS+22
   BNE   .jcmp_skip@8
   LDA   HARGS+21
   BNE   .jcmp_skip@8
   LDA   HARGS+20
   BNE   .jcmp_skip@8
   LDA   HARGS+19
   BNE   .jcmp_skip@8
   LDA   HARGS+18
   BNE   .jcmp_skip@8
   LDA   HARGS+17
   BNE   .jcmp_skip@8
   LDA   HARGS+16
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@8:
   LDA   #$05
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8
.if_end@4:
   JSR   target_eq_false
   JSR   main@run1
   BEQ   .if_end@5
   LDA   #$06
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9
.if_end@5:
   JSR   target_eq_true
   JSR   main@run1
   BNE   .if_end@6
   LDA   #$07
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10
.if_end@6:
   JSR   target_neq_false
   JSR   main@run1
   BEQ   .if_end@7
   LDA   #$08
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11
.if_end@7:
   JSR   target_neq_true
   JSR   main@run1
   BNE   .if_end@8
   LDA   #$09
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12
.if_end@8:
   JSR   target_gt_false
   JSR   main@run1
   BEQ   .if_end@9
   LDA   #$0A
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13
.if_end@9:
   JSR   target_gt_true
   JSR   main@run1
   BNE   .if_end@10
   LDA   #$0B
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14
.if_end@10:
   JSR   target_ge_false
   JSR   main@run1
   BEQ   .if_end@11
   LDA   #$0C
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15
.if_end@11:
   JSR   target_ge_true
   JSR   main@run1
   BNE   .if_end@12
   LDA   #$0D
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16
.if_end@12:
   JSR   target_lt_false
   JSR   main@run1
   BEQ   .if_end@13
   LDA   #$0E
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17
.if_end@13:
   JSR   target_lt_true
   JSR   main@run1
   BNE   .if_end@14
   LDA   #$0F
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18
.if_end@14:
   JSR   target_le_false
   JSR   main@run1
   BEQ   .if_end@15
   LDA   #$10
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18@tail19
.if_end@15:
   JSR   target_le_true
   JSR   main@run1
   BNE   .if_end@16
   LDA   #$11
   JSR   main@run0
   JMP   target_add@tail2@tail4@tail7@tail8@tail9@tail10@tail11@tail12@tail13@tail14@tail15@tail16@tail17@tail18@tail19@tail20
.if_end@16:
   JMP   target_eq_false@tail1

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

   JSR   target_rem@tail0@tail5@run2
   STY   HARGS+19
   STY   HARGS+20
   RTS

val_to_negate:
   DC.L  $7FFFFFFD
   DC.L  $00000000
