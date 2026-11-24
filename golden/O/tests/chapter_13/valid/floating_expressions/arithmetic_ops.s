SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_complex_expression__0	EQU	$82
__local_complex_expression__1	EQU	$83
__local_complex_expression__2	EQU	$84
__local_complex_expression__3	EQU	$85
__local_complex_expression__4	EQU	$86
__local_complex_expression__5	EQU	$87
__local_complex_expression__6	EQU	$88
__local_complex_expression__7	EQU	$89

; @zp-link-meta-begin
; def addition params= locals=__local_addition__0,__local_addition__1,__local_addition__2,__local_addition__3,__local_addition__4,__local_addition__5,__local_addition__6,__local_addition__7 indirect=false in_cycle=false
; def complex_expression params= locals=__local_complex_expression__0,__local_complex_expression__1,__local_complex_expression__2,__local_complex_expression__3,__local_complex_expression__4,__local_complex_expression__5,__local_complex_expression__6,__local_complex_expression__7,__local_complex_expression__8,__local_complex_expression__9,__local_complex_expression__10,__local_complex_expression__11,__local_complex_expression__12,__local_complex_expression__13,__local_complex_expression__14,__local_complex_expression__15 indirect=false in_cycle=false
; def division params= locals=__local_division__0,__local_division__1,__local_division__2,__local_division__3,__local_division__4,__local_division__5,__local_division__6,__local_division__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def multiplication params= locals=__local_multiplication__0,__local_multiplication__1,__local_multiplication__2,__local_multiplication__3,__local_multiplication__4,__local_multiplication__5,__local_multiplication__6,__local_multiplication__7 indirect=false in_cycle=false
; def negation params= locals=__local_negation__0,__local_negation__1,__local_negation__2,__local_negation__3,__local_negation__4,__local_negation__5,__local_negation__6,__local_negation__7 indirect=false in_cycle=false
; def subtraction params= locals=__local_subtraction__0,__local_subtraction__1,__local_subtraction__2,__local_subtraction__3,__local_subtraction__4,__local_subtraction__5,__local_subtraction__6,__local_subtraction__7 indirect=false in_cycle=false
; call main -> addition
; call main -> complex_expression
; call main -> division
; call main -> multiplication
; call main -> negation
; call main -> subtraction
; item addition section=.text local=false zero=false
; item subtraction section=.text local=false zero=false
; item multiplication section=.text local=false zero=false
; item division section=.text local=false zero=false
; item division@run0 section=.text local=true zero=false
; item negation section=.text local=false zero=false
; item complex_expression section=.text local=false zero=false
; item main section=.text local=false zero=false
; item point_one section=.data local=false zero=false
; item point_two section=.data local=false zero=false
; item point_three section=.data local=false zero=false
; item two section=.data local=false zero=false
; item three section=.data local=false zero=false
; item four section=.data local=false zero=false
; item twelveE30 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

addition:
   SUBROUTINE

   LDA   point_one
   STA   HARGS
   LDA   point_one+1
   STA   HARGS+1
   LDA   point_one+2
   STA   HARGS+2
   LDA   point_one+3
   STA   HARGS+3
   LDA   point_one+4
   STA   HARGS+4
   LDA   point_one+5
   STA   HARGS+5
   LDA   point_one+6
   STA   HARGS+6
   LDA   point_one+7
   STA   HARGS+7
   LDA   point_two
   STA   HARGS+8
   LDA   point_two+1
   STA   HARGS+9
   LDA   point_two+2
   STA   HARGS+10
   LDA   point_two+3
   STA   HARGS+11
   LDA   point_two+4
   STA   HARGS+12
   LDA   point_two+5
   STA   HARGS+13
   LDA   point_two+6
   STA   HARGS+14
   LDA   point_two+7
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+23
   CMP   #$3F
   BNE   .thread@0
   LDA   HARGS+22
   CMP   #$D3
   BNE   .thread@0
   LDA   HARGS+21
   CMP   #$33
   BNE   .thread@0
   LDA   HARGS+20
   CMP   #$33
   BNE   .thread@0
   LDA   HARGS+19
   CMP   #$33
   BNE   .thread@0
   LDA   HARGS+18
   CMP   #$33
   BNE   .thread@0
   LDA   HARGS+17
   CMP   #$33
   BNE   .thread@0
   LDA   HARGS+16
   CMP   #$34
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

subtraction:
   SUBROUTINE

   LDA   four
   STA   HARGS
   LDA   four+1
   STA   HARGS+1
   LDA   four+2
   STA   HARGS+2
   LDA   four+3
   STA   HARGS+3
   LDA   four+4
   STA   HARGS+4
   LDA   four+5
   STA   HARGS+5
   LDA   four+6
   STA   HARGS+6
   LDA   four+7
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
   JSR   dsub
   LDA   HARGS+23
   CMP   #$40
   BNE   .thread@1
   LDA   HARGS+22
   CMP   #$08
   BNE   .thread@1
   LDA   HARGS+21
   BNE   .thread@1
   LDA   HARGS+20
   BNE   .thread@1
   LDA   HARGS+19
   BNE   .thread@1
   LDA   HARGS+18
   BNE   .thread@1
   LDA   HARGS+17
   BNE   .thread@1
   LDA   HARGS+16
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

multiplication:
   SUBROUTINE

   LDA   #$7B
   STA   HARGS
   LDA   #$14
   STA   HARGS+1
   LDA   #$AE
   STA   HARGS+2
   LDA   #$47
   STA   HARGS+3
   LDA   #$E1
   STA   HARGS+4
   LDA   #$7A
   STA   HARGS+5
   LDA   #$84
   STA   HARGS+6
   LDA   #$3F
   STA   HARGS+7
   LDA   point_three
   STA   HARGS+8
   LDA   point_three+1
   STA   HARGS+9
   LDA   point_three+2
   STA   HARGS+10
   LDA   point_three+3
   STA   HARGS+11
   LDA   point_three+4
   STA   HARGS+12
   LDA   point_three+5
   STA   HARGS+13
   LDA   point_three+6
   STA   HARGS+14
   LDA   point_three+7
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+23
   CMP   #$3F
   BNE   .thread@2
   LDA   HARGS+22
   CMP   #$68
   BNE   .thread@2
   LDA   HARGS+21
   CMP   #$93
   BNE   .thread@2
   LDA   HARGS+20
   CMP   #$74
   BNE   .thread@2
   LDA   HARGS+19
   CMP   #$BC
   BNE   .thread@2
   LDA   HARGS+18
   CMP   #$6A
   BNE   .thread@2
   LDA   HARGS+17
   CMP   #$7E
   BNE   .thread@2
   LDA   HARGS+16
   CMP   #$FA
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

division:
   SUBROUTINE

   JSR   division@run0
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$1C
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   LDA   two
   STA   HARGS+8
   LDA   two+1
   STA   HARGS+9
   LDA   two+2
   STA   HARGS+10
   LDA   two+3
   STA   HARGS+11
   LDA   two+4
   STA   HARGS+12
   LDA   two+5
   STA   HARGS+13
   LDA   two+6
   STA   HARGS+14
   LDA   two+7
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+23
   CMP   #$40
   BNE   .thread@3
   LDA   HARGS+22
   CMP   #$0C
   BNE   .thread@3
   LDA   HARGS+21
   BNE   .thread@3
   LDA   HARGS+20
   BNE   .thread@3
   LDA   HARGS+19
   BNE   .thread@3
   LDA   HARGS+18
   BNE   .thread@3
   LDA   HARGS+17
   BNE   .thread@3
   LDA   HARGS+16
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
   RTS

division@run0:
   SUBROUTINE

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   RTS

negation:
   SUBROUTINE

   LDA   twelveE30
   STA   HARGS+8
   LDA   twelveE30+1
   STA   HARGS+9
   LDA   twelveE30+2
   STA   HARGS+10
   LDA   twelveE30+3
   STA   HARGS+11
   LDA   twelveE30+4
   STA   HARGS+12
   LDA   twelveE30+5
   STA   HARGS+13
   LDA   twelveE30+6
   STA   HARGS+14
   LDA   twelveE30+7
   EOR   #$80
   STA   HARGS+15
   LDA   #$AF
   STA   HARGS
   LDA   #$69
   STA   HARGS+1
   LDA   #$38
   STA   HARGS+2
   LDA   #$EB
   STA   HARGS+3
   LDA   #$C2
   STA   HARGS+4
   LDA   #$EE
   STA   HARGS+5
   LDA   #$62
   STA   HARGS+6
   LDA   #$46
   STA   HARGS+7
   JSR   dadd
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   ORA   HARGS+22
   ORA   HARGS+23
   BEQ   .lnot_true@12
   LDA   #$00
   BEQ   .lnot_end@13
.lnot_true@12:
   LDA   #$01
.lnot_end@13:
   LDX   #$00
   RTS

complex_expression:
   SUBROUTINE

   LDA   two
   STA   HARGS
   LDA   two+1
   STA   HARGS+1
   LDA   two+2
   STA   HARGS+2
   LDA   two+3
   STA   HARGS+3
   LDA   two+4
   STA   HARGS+4
   LDA   two+5
   STA   HARGS+5
   LDA   two+6
   STA   HARGS+6
   LDA   two+7
   STA   HARGS+7
   LDA   three
   STA   HARGS+8
   LDA   three+1
   STA   HARGS+9
   LDA   three+2
   STA   HARGS+10
   LDA   three+3
   STA   HARGS+11
   LDA   three+4
   STA   HARGS+12
   LDA   three+5
   STA   HARGS+13
   LDA   three+6
   STA   HARGS+14
   LDA   three+7
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   STA   __local_complex_expression__7
   LDA   HARGS+17
   STA   __local_complex_expression__6
   LDA   HARGS+18
   STA   __local_complex_expression__5
   LDA   HARGS+19
   STA   __local_complex_expression__4
   LDA   HARGS+20
   STA   __local_complex_expression__3
   LDA   HARGS+21
   STA   __local_complex_expression__2
   LDA   HARGS+22
   STA   __local_complex_expression__1
   LDA   HARGS+23
   STA   __local_complex_expression__0
   JSR   division@run0
   STA   HARGS+4
   LDA   #$E0
   STA   HARGS+5
   LDA   #$5F
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   LDA   four
   STA   HARGS+8
   LDA   four+1
   STA   HARGS+9
   LDA   four+2
   STA   HARGS+10
   LDA   four+3
   STA   HARGS+11
   LDA   four+4
   STA   HARGS+12
   LDA   four+5
   STA   HARGS+13
   LDA   four+6
   STA   HARGS+14
   LDA   four+7
   STA   HARGS+15
   JSR   dmul
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
   LDA   __local_complex_expression__7
   STA   HARGS
   LDA   __local_complex_expression__6
   STA   HARGS+1
   LDA   __local_complex_expression__5
   STA   HARGS+2
   LDA   __local_complex_expression__4
   STA   HARGS+3
   LDA   __local_complex_expression__3
   STA   HARGS+4
   LDA   __local_complex_expression__2
   STA   HARGS+5
   LDA   __local_complex_expression__1
   STA   HARGS+6
   LDA   __local_complex_expression__0
   STA   HARGS+7
   JSR   dsub
   LDA   HARGS+23
   CMP   #$C0
   BNE   .thread@4
   LDA   HARGS+22
   CMP   #$7F
   BNE   .thread@4
   LDA   HARGS+21
   CMP   #$90
   BNE   .thread@4
   LDA   HARGS+20
   BNE   .thread@4
   LDA   HARGS+19
   BNE   .thread@4
   LDA   HARGS+18
   BNE   .thread@4
   LDA   HARGS+17
   BNE   .thread@4
   LDA   HARGS+16
   BEQ   .cmp_true@14
.thread@4:
   LDA   #$00
   BEQ   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   JSR   addition
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   subtraction
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   multiplication
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   division
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   negation
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   complex_expression
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

point_one:
   DC.L  $9999999A
   DC.L  $3FB99999

point_two:
   DC.L  $9999999A
   DC.L  $3FC99999

point_three:
   DC.L  $33333333
   DC.L  $3FD33333

two:
   DC.L  $00000000
   DC.L  $40000000

three:
   DC.L  $00000000
   DC.L  $40080000

four:
   DC.L  $00000000
   DC.L  $40100000

twelveE30:
   DC.L  $EB3869AF
   DC.L  $4662EEC2
